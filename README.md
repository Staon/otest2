# otest2

_OTest2_ is a C++ framework for writing of automated tests (unit tests, integration tests, etc.).

Design goals of the framework are:

1. getting rid of writing boiler plate code,
2. extensibility of the framework for different development environments,
3. basic usability in IDEs.

The first goal is reached by a processor based on the _libclang_ which offers full C++ parser. Hence, there are
no limits how the tests are written. Furthermore, the full parser allows many advanced features like fixture importing
and assserted try/catch blocks.

In the sake of the second goal, the framework allows to make own implementation of several crucial parts: user can
define own assertions, own catcher of unhandled exceptions, own main loop or own test reporter. So the framework can
be easily integrated into your specific development environment.

The last goal means working IDE functions like code assistance with the test DSL files. The test files are valid
C++ code so everything works instantly.

## How to build

The framework strongly depends on the _libclang_ library. Unfortunatelly, there are no good packages of it and
the library must be build manually.

### Building Clang

1. Download llvm and clang in version 7.X.X from the [LLVM project site](http://releases.llvm.org/) (different
   version will work very unlikely).
2. Unpack both archives. The unpacked clang must be placed just **next** the unpacked LLVM and must be renamed to _clang_.
3. Create another directory (for example _build_) and change current working dir to.
4. Run cmake: 
   ```
   cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_USE_LINKER=gold path_to_llvm
   ```
   Note: don't try to build the _Debug_ type unless you have much much memory.
5. Then build the libraries:
   ```
   make
   ````
6. At the end install the clang somewhere
   ```
   cmake -DCMAKE_INSTALL_PREFIX=somewhere_path -P cmake_install.cmake
   ```
### Building OTest2

Building of the framework is now easy:
```
scons --with-llvm=path-to-llvm-config
```
The _path-to-llvm-config_ is a path to the _llvm-config_ binary. It is installed in the _bin_ subdirectory of the installed
clang project. If the utility can be found by searching in the _PATH_ variable the switch may be omitted.

If you want to run the selftest suite use this command
```
scons --with-llvm=path-to-llvm-config check
```
The processor needs to know paths to system headers. If compilation of the test suite fails try to fill correct paths
in the root _SConstruct_ file.

## How to use

### Quick overview - Hello world

Let's get a simple example. Create file _hello_world.ot2_:
```c++
#include <otest2/otest2.h>

namespace HelloWorld {

TEST_SUITE(HelloWorldSuite) {
  TEST_CASE(HelloWorldCase) {
    TEST_SIMPLE() {
      testAssertEqual("Hello World", "Hello World 2");
    }
  }
}

} /* -- namespace HelloWorld */
```
This is a very simple test description. It defines test suite _HelloWorldSuite_ containing one test case _HelloWorldCase_.
The test can be optionaly placed in a namespace(s), here the _HelloWorld_ namespace is used. Inside the case one assertion
is used - comparison of two values. The values are different, hence the test must fail.

Don't bother by the _TEST_SIMPLE()_ mark. At this point just accept it must be here.

Now apply the OTest2 processor on the test description:
```
otest2 -Ipath_to_otest2_headers -Ipath_to_system_headers hello_world.ot2
```
The processor generates new file _hello_world.cpp_. It contains the code of the test as well as other generated code
making registrations of the suite and the case.

To run the test we must write the _main_ function. The framework doesn't offer anyone as the function can differ
in different development environments. In our example, create a file _main.cpp_:
```c++
#include <otest2/dfltenvironment.h>
#include <otest2/dfltloop.h>

int main(
    int argc_,
    char* argv_[]) {
  ::OTest2::DfltEnvironment environment_(argc_, argv_);
  return ::OTest2::defaultMainLoop(environment_.getRunner());
}
```
I strongly suggest creation of a code generator in you building system instead of copying of the test main file.

Now compile it together:
```
g++ -Ipath_to_otest2_headers -std=c++11 -o hello_world hello_world.cpp main.cpp -Lpath_to_otest2_libs -lotest2 -lncurses
```
If everything goes OK a _hello_world_ binary will be created. Then if you run it you should get similar output:
```
 ============================== HelloWorldSuite ===============================
[hello_world.ot2:26] relation 'a == b' has failed: a = 'Hello World', b = 'Hello World 2'
  HelloWorldCase                                                      [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    0                   1                   1
  Checks                   0                   1                   1
  Test total                                                          [Failed]
 ==============================================================================
```
### More complicated - test fixtures

The OTest2 framework supports test fixtures. Fixtures can be used either in test cases or in test suites, shared
by all test cases in the suite. A fixture variable can be initialized by its constructor or it can be initialized
in the _startUp_ framework event. Similarly, destruction can be done by destructor or in the _tearDown_ event.

Let's get an example:
```c++
#include <otest2/otest2.h>

#include <string>

TEST_SUITE(Fixtures) {
  std::string suite_fixture1("suite fixture 1");
  std::string suite_fixture2;
  std::string overload("suite value");

  TEST_START_UP() {
    suite_fixture2 = "suite fixture 2";
  }

  TEST_TEAR_DOWN() {
    suite_fixture2 = "";
  }

  TEST_CASE(Access) {
    std::string case_fixture1("case fixture 1");
    std::string case_fixture2;
    std::string overload("case value");

    TEST_START_UP() {
      case_fixture2 = "case fixture 2";
    }

    TEST_TEAR_DOWN() {
      case_fixture2 = "";
    }

    TEST_SIMPLE() {
      testAssertEqual(suite_fixture1, "suite fixture 1");
      testAssertEqual(suite_fixture2, "suite fixture 2");
      testAssertEqual(case_fixture1, "case fixture 1");
      testAssertEqual(case_fixture2, "case fixture 2");
      testAssertEqual(overload, "case value");
    }
  }
}
```
In the example the variables _suite_fixture1_ and _case_fixture1_ are initialized/destructed by the constructor
and destructor. The round brackets are mandatory, _OTest2_ processor supports neither curly braces nor the assignment
initialization.

The variables _suite_fixture2_ and _case_fixture2_ are initialized in the _startUp_ event and destructed in
the _tearDown_ event. However, the variables are initialized by the default constructor and destructed by
the destructor too while the testing objects are created.

The _OTest2_ framework keeps a precise lifetime cycle. The test suite object or the test case object is created
just prior to the entering and it's destroyed immediately after leaving. User can be assured that all fixtures
of a test case are destroyed (invoked destructors) before entering of next test case. Similarly, all suite fixtures
are destroyed prior entering next test suite.

The variable _overload_ shows a possibility to overload a suite fixture in a test case. In the scope of the test
case the overloaded variable is used. The suite variable keeps untouched and unchanged.

The order of directives is mandtory. Firstly, the fixture variables must be declared including optional initialization.
Then the _startUp_ event must be declared and then the _tearDown_ event must be declared. All previous steps are optional.
At the end, the content of the test object (suite, case) is declared (nested test case or the simple test directive).

# otest2

## Table of content

1. [Introduction](#introduction)
2. [How to build](#how-to-build)
   1. [Building Clang](#building-clang)
   2. [Building OTest2](#building-otest2)
3. [How to use](#how-to-use)
   1. [Quick Overview - Hello World](#quick-overview---hello-world)
   2. [More Complicated - Test fixtures](#more-complicated---test-fixtures)
   3. [Asserted try/catch blocks](#asserted-trycatch-blocks)
   4. [Main Loop](#main-loop)

## Introduction

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

### Asserted try/catch blocks

How to test whether an exception is thrown? The _OTest2_ framework offers a simple construction looking at first sight
like an ordinary try/catch block. However, there is one important difference: the exception must happen. The _OTest2_
preprocessor inserts code which checks the occurence of the exception. If the exception doesn't happen, the test
case will fail. See an example:

```c++
#include <otest2/otest2.h>

#include <exception>

TEST_SUITE(Fixtures) {
  TEST_CASE(MainLoop) {
    TEST_SIMPLE() {
      testTry {
        throw std::bad_exception();
      }
      testCatch(std::bad_exception&, exc_) {
        testAssertEqual(exc_.what(), "std::bad_exception");
      }
    }
  }
}
```
Now try to remove the exception throwing. The test should fail.

Generally, any unhandled exception thrown from a test case is considered being an error causing failure of the test case.
If the unhandled exception is derived from the _std::exception_ class, the framework is able to print the error
message (the _what()_ method). Otherwise, it only reports _unexpected unknown exception_. If your environment doesn't
derive exceptions from the _std::exception_ class, you can make own implementation of the _ExcCatcher_ interface
and inject it into the test environment.

### Main Loop

Try to imagine you have to create an integration test which sends some command to another service. The service is supposed
to do something asynchronously and after a while it should notify a message back. This simple model can be a problem in
standard testing frameworks. Because the frameworks fully control run of the testing binary, user has limited chance
to integrate own event loop, which can take the back-message: the user can run another thread, or he can enter the own main
loop recursively. If his library cannot handle threads correctly or if his main loop doesn't support repeated entering,
there is no other option.

The _OTest2_ supports integration of your own event loop. The framework offers an interface, which is designed to be
invoked again and again until the test run is finished. And which can be called from almost any event loop one can
imagine. There is only one request: a timer with millisecond precision is needed.

Let see an example. Following code is a simple skeleton how to integrate the framework with the _libevent_ main
event loop:

```c++
#include <event2/event.h>
#include <otest2/dfltenvironment.h>
#include <otest2/runner.h>
#include <sys/time.h>

namespace {

struct Loop {
    ::OTest2::DfltEnvironment* environment;
    event_base* base;
    event* ev;
    int result_code;
};

void scheduleTimer(
    Loop* loop_,
    int delay_) {
  struct timeval period_ = {delay_ / 1000, (delay_ % 1000) * 1000000};
  event_add(loop_->ev, &period_);
}

void timerCallback(
    evutil_socket_t,
    short,
    void* udata_) {
  Loop* loop_(static_cast<Loop*>(udata_));

  ::OTest2::RunnerResult result_(loop_->environment->getRunner().runNext());
  if(!result_.isFinished()) {
    /* -- schedule next test step */
    scheduleTimer(loop_, result_.getDelayMS());
  }
  else {
    /* -- stop the mainloop */
    if(result_.getResult())
        loop_->result_code =  0;
      else
        loop_->result_code = 1;
    event_base_loopbreak(loop_->base);
  }
}

} /* -- namespace */

int main(
    int argc_,
    char* argv_[]) {
  /* -- prepare the testing environment */
  ::OTest2::DfltEnvironment environment_(argc_, argv_);

  /* -- initialize the main loop */
  Loop loop_;
  loop_.environment = &environment_;
  loop_.base = event_base_new();
  loop_.ev = event_new(loop_.base, -1, 0, timerCallback, &loop_);
  loop_.result_code = 0;
  scheduleTimer(&loop_, 0);  /* -- first waking up */

  /* -- enter the main loop */
  event_base_dispatch(loop_.base);

  /* -- clean up the main loop */
  event_free(loop_.ev);
  event_base_free(loop_.base);

  return loop_.result_code;
}
```

Look at the _timerCallback_ function. The method _runNext()_ of the interface _Runner_ is invoked. The returned
value is a simple structure. If the test is not finished yet, a delay prior next run is got and a timer is scheduled.
If the test is finished, a result (failed or passed) of the entire test is returned.

However, there is another question. How to break a running test case and return back into the main loop? The _OTest2_
framework offers another level in its data model. Every test case contains one or more _test states_. Transitions
between states mean returning back into the main loop and waiting for specified time. See the example:

```c++
#include <otest2/otest2.h>

TEST_SUITE(AsynchronousReplySuite) {
  TEST_CASE(AynchronousReply) {
    TEST_STATE(Ending);

    TEST_STATE(Enter) {
      /* ... send the message ... */
      
      /* -- wait one second for the asynchronous reply */
      switchState(Ending, 1000);
    }

    TEST_STATE(Ending) {
      /* ... check whether the message has actually arrived ... */
    }
  }
}
```

One test case runs until no next state is scheduled. The sequence of transitions can be complicated and states can
be repeated. First defined state (the _Enter_ in the example) is handled as the test state which is run after first
entering into the test case.

Now the reader is able to understand meaning of the _TEST_SIMPLE()_ directive: it's a default test state used by simple
test cases containing only one test state.

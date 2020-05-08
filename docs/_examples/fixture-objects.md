---
title: Fixture Objects
lesson: 7
---
Objects which are part of the fixture initialization.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/fixture-objects)

As it has been shown in the [fixture example]({{ "/examples/fixtures" | relative_url }})
there are two phases of initialization of fixtures of a testing object.
During the first phase the fixture variables are created by their constructor
methods. The second phase invokes the start-up method of the testing object.

Actually, the second phase is not just invocation of the start-up method.
The framework invokes all start-up methods of fixture variables which
declare some. Let's get a simple example:

```c++
#include <otest2/otest2.h>

#include <iostream>

namespace {

class MyFixture {
  public:
    void startUp(
        const Context& context_) OT2_START_UP();
    void tearDown() OT2_TEAR_DOWN();
};

void MyFixture::startUp(
    const Context& context_) {
  std::cout << "my fixture has just started" << std::endl;
}

void MyFixture::tearDown() {
  std::cout << "my fixture has just stopped" << std::endl;
}

} /* -- namespace */

TEST_SUITE(FixtureObjectsSuite) {
  TEST_CASE(FixtureObjects) {
    MyFixture fixture_object;
    
    TEST_SIMPLE() {
      std::cout << "test" << std::endl;
    }
  }
}
```

After run of the test we'll get:

```plaintext
 ============================ FixtureObjectsSuite =============================
my fixture has just started
test
my fixture has just stopped
  FixtureObjects                                                      [Passed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Passed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   1                   0                   1
  Cases                    1                   0                   1
  Checks                   0                   0                   0
  Errors                                                           0
  Test total                                                          [Passed]
 ==============================================================================
```

As you can see the annotated object methods are invoked. As any testing function
the start-up and tear-down methods may accept the OTest2 context and any
user data parameters ([see the example]({{ "/examples/userdata" | relative_url }})).

The framework is able to access the fixture object through a reference,
pointer or a smart pointer. The start-up method of the testing object is invoked
before fixture objects, hence the fixture object can be created here:

```c++
TEST_SUITE(FixtureObjectsSuite) {
  std::shared_ptr<MyFixture> my_fixture;
  
  TEST_START_UP() {
    my_fixture = std::make_shared<MyFixture>();
  }
  
  /* ... */
}
```

Why to use this kind of initialization instead of the constructor methods?
It allows to access internal framework objects, mainly the stack
of commands. The fixture object can schedule a command, return back into
the main loop and wait for some asynchronous notification. E.g. if the fixture
object prepares another service needed for an integration test, it can wait until
the service starts eventually.

### Gotchas

* Inheritance (even multiple) is supported, a fixture object may be inherited
  from several other fixture objects. The framework invokes start-up and
  tear-down methods of parent classes too. However, inheritance diamonds are
  not supported. The generated source will not be compilable.
* The start-up and tear-down methods of parent classes must have different
  names. Otherwise, the generated code will not be compilable.
* The pre-processor handles inheritance, but it doesn't handle overriding
  of methods. Annotate an overriden method just in one class! Otherwise,
  the overriden method will be invoked several times!

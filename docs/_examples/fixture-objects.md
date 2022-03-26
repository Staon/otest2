---
title: Fixture Objects
lesson: 8
---
Objects which are part of the fixture initialization.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/fixture-objects)

Even though the test suites and cases are fixtures themselves, user is able to
create custom fixture objects. If the fixture object is declared as a member
of any testing object (suite or case) it's initialized during first and second
phase as well as the parent suite or case is initialized.

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
pointer or a smart pointer. The start-up method of the parent testing object
is invoked before start-up method of fixture objects, hence the fixture object
can be created here:

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
  of methods. Annotate an overridden method just in one class! Otherwise,
  the overridden method will be invoked several times!

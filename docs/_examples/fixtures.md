---
title: Fixtures
lesson: 2
---
An example showing usage of test fixtures in the _OTest2_ framework.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/fixtures)

In the world of unit-tests the term fixture means any preparation needed
for run of the test like construction of the testing environment (insertion
of mock dependencies), filling data into a database, generating of files used
as inputs of the tests etc. And, of course, cleaning them at the end of the test.

The OTest2 framework supports fixtures at the both levels of suites and testcases.
There are two phases of fixture initialization:
1. the fixture variables are created by their constructor methods
2. the start-up method of the testing object is invoked.

Complementarily, destruction of fixture variables is done in two symmetric phases:
1. the tear-down method of the testing object is invoked
2. the fixture variables are destroyed by their destructor methods. 

The first phase is the natural C++ way - the fixture variables are just
member variable. As the framework creates any testing object just before
its entering and destructs the object instantly at its end, the fixture variables
exist just for run of the testing object[^1]. 

The start-up and tear-down methods are just member functions of a testing
object invoked at the beginning and at the end. The framework promises that
the tear-down method is called every time even if the test object fails
or ends up with an unhandled exception.

OK, let's get an example:

{% highlight c++ %}
#include <otest2/otest2.h>

#include <iostream>
#include <memory>

#include "myfixture.h"

namespace OTest2 {

namespace Examples {

namespace Test {

TEST_SUITE(Fixtures) {
  /* -- This is a suite's fixture initialized in constructor. The round
   *    braces are mandatory - OTest2 doesn't support other syntax of
   *    initializers. */
  MyFixture fixture1("Number one");

  /* -- This is a suite's fixture initialized in the start-up method. */
  std::unique_ptr<MyFixture> fixture2;

  TEST_START_UP() {
    /* -- This method is invoked at the beginning of the suite. The second
     *    fixture is created here. */
    fixture2.reset(new MyFixture("Number two"));
  }

  TEST_TEAR_DOWN() {
    /* -- This method is invoked at the end of the suite. The second fixture
     *    is explicitly destructed here. */
    fixture2.reset(nullptr);
  }

  TEST_CASE(FirstCase) {
    /* -- Fixtures of the first case. */
    MyFixture fixture3("Number three");
    std::unique_ptr<MyFixture> fixture4;

    TEST_START_UP() {
      fixture4.reset(new MyFixture("Number four"));
    }

    TEST_TEAR_DOWN() {
      fixture4.reset(nullptr);
    }

    TEST_SIMPLE() {
      std::cout << "enter case 1" << std::endl;

      testAssert(fixture1.checkName("Number one"));
      testAssert(fixture2->checkName("Number two"));
      testAssert(fixture3.checkName("Number three"));
      testAssert(fixture4->checkName("Number four"));

      std::cout << "leave case 1" << std::endl;

      throw "An exception - the fixtures are cleaned up anyway";
    }
  }

  TEST_CASE(SecondCase) {
    /* -- This fixture shadows the suite's fixture. In the whole testcase
     *    this variable is used instead of the suite's one. */
    MyFixture fixture1("Number five");

    TEST_SIMPLE() {
      std::cout << "enter case 2" << std::endl;

      testAssert(fixture1.checkName("Number five"));
      testAssert(fixture2->checkName("Number two"));

      std::cout << "leave case 2" << std::endl;
    }
  }
}

} /* -- namespace Test */

} /* -- namespace Examples */

} /* -- namespace OTest2 */
{% endhighlight %}

If you compile and run this test you'll got similar output (without added line numbers):
{% highlight plaintext linenos %}
 ================================== Fixtures ==================================
started 'Number one'
started 'Number two'
started 'Number three'
started 'Number four'
enter case 1
leave case 1
error in fixtures::Fixtures::FirstCase: unexpected unknown exception
stopped 'Number four'
stopped 'Number three'
  FirstCase                                                           [Failed]
started 'Number five'
enter case 2
leave case 2
stopped 'Number five'
  SecondCase                                                          [Passed]
stopped 'Number two'
stopped 'Number one'
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    1                   1                   2
  Checks                   6                   0                   6
  Errors                                                           1
  Test total                                                          [Failed]
 ==============================================================================
{% endhighlight %}

* line 02: the test suite object is created and _fixture1_ is initialized in its constructor.
* line 03: the start-up method of the suite object is invoked and _fixture2_ is initialized in.
* line 04: the test case object is created and _fixture3_ is initialized in its constructor.
* line 05: the start-up method of the test case object is invoked and _fixture4_ is created in.
* line 06: the test case body is run.
* line 08: the test case ends up by an exception. The framework catches it and reports failure.
* line 09: the tear-down method of the test case object is invoked. _fixture4_ is destroyed in.
* line 10: the test case object is destroyed. During its destruction _fixture3_ is destroyed too.
* line 11: the result of the destroyed test case object is reported.
* line 12: the second test case is created and the shadowing _fixture1_ too.
* line 15: the tear-down method of the second test case is invoked and the shadowing _fixture1_ is destroyed.
* line 16: the result of the second test case is reported.
* line 17: the tear-down method of the suite is invoked and the _fixture2_ is destroyed in.
* line 18: the suite object is destroyed. During its destruction _fixture1_ is destroyed too.
* line 19: the result of the suite object is reported.

[^1]: The framework keeps precise lifetime of testing objects. This is one
      of the advantages using the libclang. 

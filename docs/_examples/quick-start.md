---
title: Quick Start
lesson: 1
---
A "Hello World" style example including integration into a cmake project.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/quick-start)

Let's get a simple example. Create file _quick.ot2_:

{% highlight c++ %}
#include <otest2/otest2.h>

TEST_SUITE(QuickStart) {
  TEST_CASE(TestOK) {
    TEST_SIMPLE() {
      testAssertEqual(10, 10);
    }
  }

  TEST_CASE(TestFailed) {
    TEST_SIMPLE() {
      testAssertEqual(10, 9);
    }
  }

  TEST_CASE(TestExpression) {
    TEST_SIMPLE() {
      testAssertExpr(std::string("hello").length() == 6);
    }
  }
}
{% endhighlight %}

The file describes one test suite _QuickStart_ containing three test cases:
_TestOK_ and _TestFailed_. The first case should pass, the second one should
fail.

The third test case shows a special assertion `testAssertExpr` which parses
relational operators in the specified expression and it's able to print
both operands. In this case the string is not 6 characters long so the
test case fails.

See [the reference page]({{ "reference/assertions/" | relative_url }})
to explore the entire rich set of assertions offered by the framework.

The OTest2 framework offers a cmake package, so the test can be easily used
in your cmake project:

{% highlight cmake %}
cmake_minimum_required(VERSION 3.7)

project(otest2-example-quick-start)

find_package(OTest2)

add_executable(quick-start "")
target_otest2_sources(quick-start
    quick.ot2
)
target_otest2_main(quick-start)
target_link_libraries(quick-start libotest2)

add_custom_target(check ALL COMMAND quick-start DEPENDS quick-start)
{% endhighlight %}

The _target_otest2_sources_ function adds otest2 sources into specified target.
For each of them the pre-processor is run and the generated C++ file is added
among other sources of the target.

The function _target_otest2_main_ generates a source file _main.cpp_ containing
the _main_ function. The generated main parses command line arguments and runs
all registered suites. As the framework allows integration with custom main
loop, user may write his very own main function suiting his needs. Then this
step is skipped.

Finally, the _libotest2_ must be linked. After successful compilation

{% highlight console %}
$ mkdir build
$ cd build
$ cmake ..
$ make
{% endhighlight %}

you should see similar output (cmake messages are removed and the actual
terminal output is colorized):
{% highlight plaintext %}
 ================================= QuickStart =================================
  TestOK                                                              [Passed]
[.../examples/quick-start/quick.ot2:31] quick-start::QuickStart::TestFailed: che
ck 'a == b' has failed
    a = 10
    b = 9
  TestFailed                                                          [Failed]
[.../examples/quick-start/quick.ot2:37] quick-start::QuickStart::TestExpression:
check 'a == b' has failed
    a = 5
    b = 6
  TestExpression                                                      [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    1                   1                   2
  Checks                   1                   1                   2
  Errors                                                           0
  Test total                                                          [Failed]
 ==============================================================================
{% endhighlight %}
As it's expected the first case passes and the second one fails.

---
title: Exception Handling
lesson: 3
---
A special **try/catch** block checking whether an exception actually happens.
Handling of unexpected exceptions fired from a test case.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/exceptions)

The framework offers a simple way how to check an exception really happens:

{% highlight c++ %}
#include <otest2/otest2.h>

#include <exception>

namespace OTest2 {

namespace Examples {

namespace Test {

TEST_SUITE(Exceptions) {
  TEST_CASE(TryCatchPassed) {
    TEST_SIMPLE() {
      testTry {
        throw std::bad_exception();
      }
      testCatch(std::exception&, exc_) {
        testAssertEqual(exc_.what(), "std::bad_exception");
      }
    }
  }

  TEST_CASE(TryCatchFailed) {
    TEST_SIMPLE() {
      testTry {
        /* -- no exception -> the test must fail */
      }
      testCatch(std::exception&, exc_) {
        testAssertEqual(exc_.what(), "std::bad_exception");
      }
    }
  }

  TEST_CASE(UnhandledException) {
    TEST_SIMPLE() {
      /* -- any exception thrown out of the test case is considered an error */
      throw std::bad_exception();
    }
  }
}

} /* -- namespace Test */

} /* -- namespace Examples */

} /* -- namespace OTest2 */
{% endhighlight %}

The **testTry/testCatch** block works as the same as the ordinary **try/catch**
directive, including the possibility to write several catch blocks. The only
difference is that the exception must happen. If not the test will fail.
The first testcase in the example catches the exception correctly and passes.
The second testcase fails because no exception happens although one is expected.

Result of the test looks like:

{% highlight plaintext %}
 ================================= Exceptions =================================
  TryCatchPassed                                                      [Passed]
[.../exceptions.ot2:43] exceptions::Exceptions::TryCatchFailed: an exception is 
expected but no one has occurred
  TryCatchFailed                                                      [Failed]
error exceptions::Exceptions::UnhandledException: unexpected exception: std::bad
_exception
  UnhandledException                                                  [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    1                   2                   3
  Checks                   2                   1                   3
  Errors                                                           1
  Test total                                                          [Failed]
 ==============================================================================
{% endhighlight %}

The third testcase shows another feature of the framework. If some exception
unwinds out of a testcase, it's caught and the case is affirmed being failed.
As you can see, the framework shows message provided by the _std::exception::what()_
method.

The default exception catcher understands a _std::exception_ object and shows
its message. If the thrown object is not a child of the _std::exception_,
some generic message will be printed. If your exceptions don't derive from
the standard exception class you can implement a custom exception catcher
and inject it into the framework - more info in the chapter 
[Extending Framework]({{ "/extending/" | relative_url }}).

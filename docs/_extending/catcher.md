---
title: Custom Exception Catcher
lesson: 3
---
Implementation of a custom catcher of unhandled exceptions.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/catcher)

The exception catcher is responsible for catching of exceptions fired unexpectedly
out of user code like a test case body or start-up and tear-down methods and
transforming of them to a message shown by the framework.

The framework offers one default implementation catching the `std::exception`
and reporting the `what()` message. If another exception is caught some
generic message is reported. Consider following test:

```c++
#include <otest2/otest2>

TEST_SUITE(ExceptionCatcher) {
  TEST_CASE(FromBody) {
    TEST_SIMPLE() {
      throw MyExceptionMsg("exception from body");
    }
  }

  TEST_CASE(FromStartUp) {
    TEST_START_UP() {
      throw MyExceptionMsg("exception from start-up");
    }

    TEST_SIMPLE() {

    }
  }

  TEST_CASE(FromTearDown) {
    TEST_TEAR_DOWN() {
      throw MyExceptionMsg("exception from tear-down");
    }

    TEST_SIMPLE() {

    }
  }

  TEST_CASE(PlainText) {
    TEST_SIMPLE() {
      throw "plain text exception";
    }
  }
}
```

An instance of `MyException`, which doesn't derive from the `std::exception`,
 is fired out of the test cases. If this test is run with the default catcher,
 the framework's output will be similar:
 
 ```plaintext
  ============================== ExceptionCatcher ==============================
error catcher::ExceptionCatcher::FromBody: unexpected unknown exception
  FromBody                                                            [Failed]
error catcher::ExceptionCatcher::FromStartUp: unexpected unknown exception
  FromStartUp                                                         [Failed]
error catcher::ExceptionCatcher::FromTearDown: unexpected unknown exception
  FromTearDown                                                        [Failed]
error catcher::ExceptionCatcher::PlainText: unexpected unknown exception
  PlainText                                                           [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    0                   4                   4
  Checks                   0                   0                   0
  Errors                                                           4
  Test total                                                          [Failed]
 ==============================================================================
```

This output is not much helpful when someone tries to find out what has actually
happened. Fortunately, the framework allows to implement your very own catcher
deriving from the interface
[`ExcCatcher`]({{ "api/html/classOTest2_1_1ExcCatcher.html" | relative_url }}).

```c++
class Catcher : public ExcCatcher {
  public:
    /* -- catcher interface */
    virtual bool catchException(
        const Context& context_,
        std::function<void(const Context&)> ftor_,
        std::string& message_) noexcept override;
};
```

The method gets the OTest2 context and a functor `ftor_`. The functor is the
user code guarded by the catcher for unexpected exceptions. The `message_`
works as an output parameter which is filled if an exception happens.
The method returns true in such situation.

```c++
bool Catcher::catchException(
    const Context& context_,
    std::function<void(const Context&)> ftor_,
    std::string& message_) noexcept {
  try {
    ftor_(context_);
    return false;
  }
  catch(MyException& exc_) {
    message_ = exc_.getMessage();
    return true;
  }
  catch(std::exception& exc_) {
    message_ = exc_.what();
    return true;
  }
  catch(const char* msg_) {
    message_ = msg_;
    return true;
  }
  catch(...) {
    message_ = "unknown exception";
    return true;
  }
}
```

If you inject the catcher into the test environment in the `main`
function

```c++
int main(
    int argc_,
    char* argv_[]) {
  /* -- prepare the testing environment */
  ::OTest2::Examples::Catcher exc_catcher_;
  ::OTest2::DfltEnvironment environment_(argc_, argv_);
  environment_.setExceptionCatcher(&exc_catcher_);

  /* -- run the test */
  return ::OTest2::defaultMainLoop(environment_.getRunner());
}
```

you'll get more descriptive output

```plaintext
 ============================== ExceptionCatcher ==============================
error in catcher::ExceptionCatcher::FromBody: exception from body
  FromBody                                                            [Failed]
error in catcher::ExceptionCatcher::FromStartUp: exception from start-up
  FromStartUp                                                         [Failed]
error in catcher::ExceptionCatcher::FromTearDown: exception from tear-down
  FromTearDown                                                        [Failed]
error in catcher::ExceptionCatcher::PlainText: plain text exception
  PlainText                                                           [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    0                   4                   4
  Checks                   0                   0                   0
  Errors                                                           4
  Test total                                                          [Failed]
 ==============================================================================
```

---
title: Custom Main Loop
lesson: 5
---
Integration of the framework with a custom main event loop.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/mainloop)

If someone writes integration tests there is always a need of communication
with other components. The communication libraries are often based on some
kind of main event loop. As standard testing frameworks keep full control
over program run their integration into an event driven environment might
be tricky.

The OTest2 supports your custom event loop. The framework doesn't control
the process flow but it offers an interface, which is designed to be invoked
again and again until the test is finished. And which can be called from almost
any event loop one can imagine. There is only one need - a timer with
millisecond precision.

In the following example we'll show integration with 
the [libevent2](https://libevent.org/) library. Firstly, we define a helper
structure which will be passed through the generic user data pointer:

{% highlight c++ %}
struct Loop {
    event_base* base;
    event* ev;
    ::OTest2::Runner* runner;
    bool result;
};
{% endhighlight %}

The structure contains a base object of the libevent library, one event
object used for scheduling of the timer, the OTest2 runner and a flag
used for reporting of the test result.

The heart of the integration is hidden in a callback method triggered
from a timer scheduled by the libevent:

{% highlight c++ %}
void timerCallback(
    evutil_socket_t,
    short,
    void* udata_) {
  Loop* loop_(static_cast<Loop*>(udata_));

  /* -- Run current test step. */
  ::OTest2::RunnerResult result_(loop_->runner->runNext());
  if(!result_.isFinished()) {
    /* -- The test is not finished yet, schedule next test step. */
    scheduleTimer(loop_, result_.getDelayMS());
  }
  else {
    /* -- The test is finished. Stop the main loop and return the test
     *    result. */
    event_base_loopbreak(loop_->base);
    loop_->result = result_.getResult();
  }
}
{% endhighlight %}

The method _::OTest2::Runner::runNext()_ returns an object describing current
state of the test. If the test is not finished yet, a timer event is scheduled
at the time requested by the test. If the test is finished, the main loop is
stopped and the test result is returned.

The rest is simple. The main function has just to prepare the testing
environment, schedule first awakening of the timer and enter the main loop.

{% highlight c++ %}
int main(
    int argc_,
    char* argv_[]) {
  /* -- Prepare the testing environment. The default environment offered
   *    by the OTest2 framework is used. */
  ::OTest2::DfltEnvironment environment_(argc_, argv_);

  /* --Initialize the main loop and schedule start of the test. */
  Loop loop_;
  loop_.base = event_base_new();
  loop_.ev = event_new(loop_.base, -1, 0, timerCallback, &loop_);
  loop_.runner = &environment_.getRunner();
  loop_.result = false;
  scheduleTimer(&loop_, 0);  /* -- first waking-up */

  /* -- enter the main loop */
  event_base_dispatch(loop_.base);

  /* -- clean up the main loop */
  event_free(loop_.ev);
  event_base_free(loop_.base);

  return loop_.result ? 0 : 1;
}
{% endhighlight %}

However, there is one question left. How to break a running test case and
return back into the main loop?

In comparison with other testing frameworks there is another level called
**test state** in the data model. One test case can contain one or more test
states. A transition between states means return back into the main loop
(actually it causes leaving of the _::OTest2::Runner::runNext()_ method).
The states can be freely switched and repeated with no limits. The test case
ends when no next state is set.

{% highlight c++ %}
#include <otest2/otest2.h>

#include <iostream>

TEST_SUITE(MainLoopExample) {
  TEST_CASE(Transition) {
    TEST_STATE(SecondState);

    TEST_STATE(FirstState) {
      /* -- Tell the framework that next state is SecondState and it should
       *    stay in the main loop for 5000 milliseconds. */
      switchState(SecondState, 5000);

      std::cout << "Entering the main loop for 5 seconds..." << std::endl;
    }

    TEST_STATE(SecondState) {
      std::cout << "I've got control again" << std::endl;
    }
  }
}
{% endhighlight %}

Now the reader should be able to understand meaning of the _TEST_SIMPLE()_
directive occurring in previous examples. It's a default anonymous test state
used by simple test cases with no need of state switching.

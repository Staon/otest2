---
title: Regression Test Marks
lesson: 4
---
Storing and checking of marks representing complex state or output of a piece
of software.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/regressions)

Regression test marks are a key feature making OTest2 different in
comparison with other testing frameworks. It allows easy checking of status
or output of complex data structures or objects with rich inner state logic.

Let's get an example. We have an object implementing simple prioritized
round-robin scheduler. There are queues of command for each priority. The very
first command in the most prioritized queue is executed. If the command
finishes it will be removed from the queue. If it doesn't finish it will
be moved at the end of the queue.

Now let's write a test.

{% highlight c++ %}
#include <otest2/otest2.h>

TEST_SUITE(Regressions) {
  TEST_CASE(RoundRobinScheduler) {
    TEST_SIMPLE() {
      RoundRobin round_robin_;

      /* -- Schedule the whole "job". The number means count of rounds
       *    until the command finishes. */
      round_robin_.scheduleCommand(1, std::make_shared<MockCommand>("A", 2));
      round_robin_.scheduleCommand(1, std::make_shared<MockCommand>("B", 1));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("C", 3));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("D", 2));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("E", 1));
    }
  }
}
{% endhighlight %}

Now we need a check that the round-robin queue is prepared as we expect[^1].
For this goal we can implement some iterator or visitor interface and check
the prepared queue. Something like this:

{% highlight c++ %}
      auto iter_(round_robin_.begin());
      testAssertEqual((*iter_).priority, 1);
      testAssertEqual((*iter_).command->getName(), "A");
      ++iter_;
      testAssertEqual((*iter_).priority, 1);
      testAssertEqual((*iter_).command->getName(), "B");
      ++iter_;
      testAssertEqual((*iter_).priority, 2);
      testAssertEqual((*iter_).command->getName(), "C");
      ++iter_;
      testAssertEqual((*iter_).priority, 2);
      testAssertEqual((*iter_).command->getName(), "D");
      ++iter_;
      testAssertEqual((*iter_).priority, 2);
      testAssertEqual((*iter_).command->getName(), "E");
      ++iter_;
      testAssertEqual(iter_, round_robin_.end());
{% endhighlight %}

Such checking of the state is a real pain in ass. Try to repeat that hundred
times after any applied scheduler's operation. If you try to wrap the checking
code into a function you have to design a special language describing the
state and to implement its parser. Fortunately, there is a solution. Teach
the round-robin scheduler to report its state as a test mark:

{% highlight c++ %}
#include <otest2/testmarkbuilder.h>

void RoundRobin::test_testMark(
    TestMarkBuilder& builder_) const noexcept {
  builder_.openList("RoundRobin");

  /* -- iterate over priorities */
  for(const auto& iter_ : scheduler) {
    builder_.openMap();
    
    /* -- report the priority */
    builder_.setKey("priority");
    builder_.appendInt(iter_.first);

    /* -- report commands stored in the queue */
    builder_.setKey("queue");
    builder_.openList();
    for(const auto& command_ : iter_.second)
      builder_.appendString(command_->getName());
    builder_.closeContainer();

    builder_.closeContainer();
  }

  builder_.closeContainer();
}
{% endhighlight %}

Then add a check into the test:

{% highlight c++ %}
#include <otest2/otest2.h>

TEST_SUITE(Regressions) {
  TEST_CASE(RoundRobinScheduler) {
    TEST_SIMPLE() {
      RoundRobin round_robin_;

      /* -- Schedule the whole "job". The number means count of rounds
       *    until the command finishes. */
      round_robin_.scheduleCommand(1, std::make_shared<MockCommand>("A", 2));
      round_robin_.scheduleCommand(1, std::make_shared<MockCommand>("B", 1));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("C", 3));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("D", 2));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("E", 1));
      testRegression("1AB2CDE", round_robin_);
    }
  }
}
{% endhighlight %}

The assertion function invokes the _test\_testMark()_ method and it compares
the built test mark with the one stored with the same key (the first argument
of the function, here it's "1AB2CDE"). If the test is run now it fails with
this output:

{% highlight plaintext %}
 ================================ Regressions =================================
[.../examples/regressions/regressions.ot2:81] regressions::Regressions::RoundRob
inScheduler: regression check 'Regressions>>RoundRobinScheduler>>1AB2CDE' has fa
iled
    ---------  Current   ---------
      RoundRobin [
        {
          priority: 1
          queue: [
            "A"
            "B"
          ]
        }
        {
          priority: 2
          queue: [
            "C"
            "D"
            "E"
          ]
        }
      ]
    ---------  Original  ---------
    --------- Difference ---------
    + RoundRobin [
    +   {
    +     priority: 1
    +     queue: [
    +       "A"
    +       "B"
    +     ]
    +   }
    +   {
    +     priority: 2
    +     queue: [
    +       "C"
    +       "D"
    +       "E"
    +     ]
    +   }
    + ]
  RoundRobinScheduler                                                 [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    0                   1                   1
  Checks                   0                   1                   1
  Errors                                                           0
  Test total                                                          [Failed]
 ==============================================================================
{% endhighlight %}

The framework reports failed test mark. As you can see it prints current
and previous marks and a difference between them. In this case the test
mark is new so the original is empty.

Now change the assertion:

{% highlight c++ %}
      testRegressionW("1AB2CDE", round_robin_);
{% endhighlight %}

and run again:

{% highlight plaintext %}
 ================================ Regressions =================================
[.../examples/regressions/regressions.ot2:81] regressions::Regressions::RoundRob
inScheduler: stored regression mark 'Regressions>>RoundRobinScheduler>>1AB2CDE'
  RoundRobinScheduler                                                 [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    0                   1                   1
  Checks                   0                   1                   1
  Errors                                                           0
  Test total                                                          [Failed]
 ==============================================================================
{% endhighlight %}

The test is still failing but the test mark is stored now. Remove the **W**
back away and re-run the test again:

{% highlight plaintext %}
 ================================ Regressions =================================
  RoundRobinScheduler                                                 [Passed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Passed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   1                   0                   1
  Cases                    1                   0                   1
  Checks                   1                   0                   1
  Errors                                                           0
  Test total                                                          [Passed]
 ==============================================================================
{% endhighlight %}

The test successfuly passes now[^2]. Now it's easy to extend the test
by checking how the state will change if first command in the queue is
invoked. Add the lines and store the mark.

{% highlight c++ %}
TEST_SUITE(Regressions) {
  TEST_CASE(RoundRobinScheduler) {
    TEST_SIMPLE() {
      RoundRobin round_robin_;

      /* -- schedule the whole "job" */
      round_robin_.scheduleCommand(1, std::make_shared<MockCommand>("A", 2));
      round_robin_.scheduleCommand(1, std::make_shared<MockCommand>("B", 1));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("C", 3));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("D", 2));
      round_robin_.scheduleCommand(2, std::make_shared<MockCommand>("E", 1));

      /* -- start: (1; A B), (2; C D E) */
      testRegression("1AB2CDE", round_robin_);

      /* -- result: (1; B A), (2; C D E) */
      round_robin_.executeNextCommand();
      testRegression("1BA2CDE", round_robin_);
    }
  }
}
{% endhighlight %}

What happens if a developer makes a mistake during doing of some changes?
For example, if he forgets to return unfinished command back to the queue?
The stored test mark will be different and so it will fail:

{% highlight plaintext %}
 ================================ Regressions =================================
[.../examples/regressions/regressions.ot2:85] regressions::Regressions::RoundRob
inScheduler: regression check 'Regressions>>RoundRobinScheduler>>1BA2CDE' has fa
iled
    ---------  Current   ---------
      RoundRobin [
        {
          priority: 1
          queue: [
            "B"
          ]
        }
        {
          priority: 2
          queue: [
            "C"
            "D"
            "E"
          ]
        }
      ]
    ---------  Original  ---------
      RoundRobin [
        {
          priority: 1
          queue: [
            "B"
            "A"
          ]
        }
        {
          priority: 2
          queue: [
            "C"
            "D"
            "E"
          ]
        }
      ]
    --------- Difference ---------
      ......... 
          priority: 1
          queue: [
            "B"
    -       "A"
          ]
        {
          priority: 2
          queue: [
      ......... 
  RoundRobinScheduler                                                 [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    0                   1                   1
  Checks                   1                   1                   2
  Errors                                                           0
  Test total                                                          [Failed]
 ==============================================================================
{% endhighlight %}

As you can see the diff output shows you that the "A" command is missing
in the new test mark.

Where the test marks are stored? By default they are in the file
_regression.ot2tm_ which is created in the working directory of the test.
It's a simple text file containing one test mark per each line. The file
must be pushed into your CVS system being kept with the specific version of your
code base. Each mark itself is bzipped and encoded to base64 and the records
are sorted by the keys in the sake of minimizing changes in the versioning
system.

### Some Advices

* The test marks are a powerful tool. On the other hand, they might be
  a dangerous self-exploding bomb. Be careful and don't overuse.
* Never project implementation details into test marks. The model expected
  by the round-robin scheduler is a set of queues ordered by priorities and
  that's exactly what the implemented mark shows. There must be no change
  even if you implement that by one priority queue of tuples _(priority, command)_
  or by a container of queues or by another tricky way.

[^1]: In the sake of example simplicity the round-robin scheduler is kept
      as simple as it's possible. In these circumstances the best way how
      to check state of the scheduler is to run the scheduled commands
      and observing their outputs. However, the real scheduler may be much
      more complex and the deeper insight into the object might be much more
      efficient approach. The real use-case in Aveco is a very complex
      scheduler keeping many dependencies, handling time intervals needed
      for preparation of techonology and grouping events into one transition.
      Still it's possible to test it by observing the output but deducing
      what has happened from an unexpected result is a huge challenge. As
      the test marks offer a deep insight into the tested object they have
      eventually become the main debugging tool. 

[^2]: Storing of test marks by a cycle "change assertion, compile, link and
      run" is not the best way. It's usable but not comfortable. In the future
      the framework is going to offer a tool for manipulating the stored marks.

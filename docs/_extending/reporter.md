---
title: Custom Reporter
lesson: 1
---
Implementation of a custom test reporter printing a dot per each test case.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/reporter)

Of course, the colorized console output is the main advantage of the OTest2
framework :-) However, there might be people who prefer the boring dot
output known from other frameworks. In this example we'll create such kind
of reporter.

Implementation of a custom reporter is easy - developer just implements
one simple interface ``::OTest2::Reporter``.

```c++
#include <otest2/reporter.h>

class ReporterDot : public Reporter {
  public:
    /* ... */
};
```

The interface contains a bunch of methods invoked when appropriate event happens.
Most of them work like opening/closing braces - e.g. `enterTest` and `leaveTest`,
`enterSuite` and `leaveSuite` etc. We are going to leave most of them empty
and to implement just couple of them:

```c++
#include "reporterdot.h"

#include <cstdio>

void ReporterDot::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& message_,
    const std::string& file_,
    int lineno_) {
  if(!condition_) {
    std::printf("\n[%s, %d]: %s\n", file_.c_str(), lineno_, message_.c_str());
  }
}

void ReporterDot::enterError(
    const Context& context_,
    const std::string& message_) {
  std::printf("\nerror: %s\n", message_.c_str());
}

void ReporterDot::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::printf(".");
  std::fflush(stdout);
}

void ReporterDot::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  if(result_)
    std::printf("OK\n");
  else
    std::printf("Failed\n");
}
```

Now the reporter must be injected into the testing environment. We can do that
in the test's _main_ function:

```c++
#include <otest2/dfltenvironment.h>
#include <otest2/dfltloop.h>

#include "reporterdot.h"

int main(
    int argc_,
    char* argv_[]) {
  /* -- construct the test environment */
  ::OTest2::Examples::ReporterDot reporter_;
  ::OTest2::DfltEnvironment environment_(argc_, argv_);
  environment_.addReporter(&reporter_);

  /* -- run the test */
  ::OTest2::defaultMainLoop(environment_.getRunner());
}
```

If we run the test now, we'll got similar output:
```plaintext
....
[.../examples/reporter/reporter.ot2, 57]: false
...Failed
```
It's awesome, isn't it?

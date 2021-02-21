---
title: Custom Reporter
lesson: 1
---
Implementation of a custom test reporter printing a dot per each test case.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/reporter)

Of course, the color console output is the main advantage of the OTest2
framework :-) However, there might be people who prefer the boring dot
output known from some frameworks. In this example we'll create such kind
of reporter.

Implementation of a custom reporter is relatively easy - just to implement
an interface [::OTest2::Reporter]({{ "api/html/classOTest2_1_1Reporter.html" | relative_url }}).

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
and to implement just a couple:

```c++
#include "reporterdot.h"

#include <cstdio>

AssertBufferPtr ReporterDot::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& file_,
    int lineno_) {
  buffer->openAssertion({condition_, file_, lineno_});
  return buffer;
}

AssertBufferPtr ReporterDot::enterError(
    const Context& context_) {
  buffer->openError();
  return buffer;
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

Methods `leaveCase` and `leaveTest` just print result of a test case
or the entire test. But be careful. The events follows the data model. So,
suites may be nested into suites, test cases may stand alone. 

More interesting methods are `enterAssert` and `enterError`. These two
are invoked if the framework wants to report some assertion or error message.
The methods return an instance of
[an assertion buffer]({{ "api/html/classOTest2_1_1AssertBuffer.html" | relative_url }}).
In this example we have used prepared implementation
[::OTest2::AssertBufferBase]({{ "api/html/classOTest2_1_1AssertBufferBase.html" | relative_url }})
and we have implemented just these event methods:

```c++
void ReporterDot::AssertBufferDot::assertionOpeningMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  if(!data_.condition)
    std::printf("\n[%s, %d]: %s\n", data_.file.c_str(), data_.line, message_.c_str());
}

void ReporterDot::AssertBufferDot::errorOpeningMessage(
    const Context& context_,
    const std::string& message_) {
  std::printf("\nerror: %s\n", message_.c_str());
}
```

The assertion buffer is an abstraction which allows to report more complex
assertion and error messages including coloring. Our simple implementation
just prints the main (first) messages and ignores the rest.

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

---
title: User Data
lesson: 6
---
Passing of the environment context into tests.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/userdata)

In [previous example]({{ "/examples/main-loop" | relative_url }}) we have shown
how to integrate the framework with your custom event loop. However, this
feature is not complete without any possibility to pass the environment,
the test is running in, into the testing code.

The framework allows passing of custom data through a generic container
[`UserData`]({{ "api/html/classOTest2_1_1UserData.html" | relative_url }}).
Firstly, user must fill wanted data into. That's as simple as one call
of the `setDatum` function of the user data container[^1].

```c++
int main(
    int argc_,
    char* argv_[]) {
  ::OTest2::Examples::MainLoop loop_;

  /* -- prepare the testing environment */
  ::OTest2::DfltEnvironment environment_(argc_, argv_);
  environment_.getUserData().setDatum("loop_", &loop_);

  /* -- run the tests */
  bool result_(loop_.runTest(&environment_.getRunner()));

  return result_ ? 0 : 1;
}
```

The first argument is a text key used for access to the user datum. It may
be any string but usually it's a C/C++ identifier.

There are two ways how to access user data in tests. The first one is to create
a fixture variable:

```c++
#include <otest2/otest2.h>

#include "mainloop.h"

OT2_SUITE(UserDataSuite) {
  OT2_CASE(FixtureVariable) {
    extern MainLoop loop OT2_USER_DATA_KEY("loop_");

    OT2_SIMPLE() {
      loop.doSomethingUseful("by fixture");
    }
  }
}
```

The macro `OT2_USER_DATA_KEY` links the fixture declaration with a user datum
identified by the specified key. If the key is equal to the name of the
variable more simple macro `OT2_USER_DATA` may be used.

The `extern` linkage is mandatory. Otherwise, the clang parser will report
an error searching for non-parametric constructor.

The second way how to access user data is function parameters. If a start-up,
tear-down or test state method has some parameters, they are expected to be
user data and the pre-processor generates code which passes user data into
the function.

```c++
#include <otest2/otest2.h>

#include "mainloop.h"

OT2_SUITE(UserDataSuite) {
  OT2_CASE(Parameters) {
    void startUp(
        MainLoop& loop_) OT2_START_UP() {
      loop_.doSomethingUseful("start-up parameter");
    }

    void tearDown(
        MainLoop& l_ OT2_USER_DATA_KEY("loop_")) OT2_TEAR_DOWN() {
      l_.doSomethingUseful("tear-down parameter");
    }

    void firstTestState(
        MainLoop& loop_) OT2_STATE() {
      loop_.doSomethingUseful("in test state");
    }
  }
}
```

As you can see the [full syntax]({{ "/reference/dsl#full-syntax" | relative_url}})
is needed as the [compact syntax]({{ "/reference/dsl#compact-syntax" | relative_url }})
doesn't allow definition of function parameters. If the name of the parameter
differs the name of the user datum the annotation `OT2_USER_DATA_KEY` can help.

If the parameter is type `const ::OTest2::Context&`
[the OTest2 Context]({{ "api/html/classOTest2_1_1Context.html" | relative_url }})
is passed into the function instead of a user datum. 

[^1]: Remark the usage of the [full syntax]({{ "/reference/dsl#full-syntax" | relative_url}}).
      All previous examples has been written using
      the [compact syntax]({{ "/reference/dsl#compact-syntax" | relative_url }}).

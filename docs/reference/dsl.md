---
title: OTest2 DSL
excerpt: Description of the OTest2 DSL
permalink: /reference/dsl/
toc: true
---

## Basic Structure

The OTest2 files are valid C++ sources using simple macros and clang annotations
to describe the test. The basic structure is quite simple:
 
```c++
#include <otest2/otest2.h>

TEST_SUITE(SuiteName) {
  /* zero or more <suite fixtures> */
  /* optional */ TEST_START_UP() { /* ... */ }
  /* optional */ TEST_TEAR_DOWN() { /* ... */ }
  /* zero or more */ TEST_CASE(CaseName) {
    /* zero or more <case fixtures> */
    /* optional */ TEST_START_UP() { /* ... */ }
    /* optional */ TEST_TEAR_DOWN() { ... }
    /* one or more */ TEST_STATE(StateName) { ... }
  }
}
```

### Fixtures

Fixtures are just member variables which may be possibly initialized. **Just
initilization by round braces** is supported only[^1]!

```c++
  std::string fixture1_;
  std::string fixture2_("Hello world");
```

See how to use the fixtures in the [example]({{ "/examples/fixtures/" | relative_url }}).

### Start-up and tear-down functions

The macros `TEST_START_UP` and `TEST_TEAR_DOWN` define the methods invoked
at the beginnig and at the end of a test object. Body of the methods
is just ordinary C++ code.

See how to use the start-up and tear-down methods in
the [example]({{ "/examples/fixtures/" | relative_url }}).

### States

States are the real testing code. One test case can contain one or more test
states and it's running until some state is scheduled. At first entering
to the test case the first defined test state is run. If you need to switch
test cases they must be declared forwardly:

```c++
    TEST_STATE(SecondState);
    
    TEST_STATE(FirstState) {
      switchState(SecondState, 100);
    }
    
    TEST_STATE(SecondState) {
    
    }
```

Usage of test states is explained in the 
[custom main loop]({{ "/examples/main-loop/" | relative_url }}) example. 

In the most situations there is only one test state. Then the macro `TEST_SIMPLE`
may be used to define one anonymous test state:

```c++
    TEST_SIMPLE() {
    
    }
```

Except ordinary C++ code the body of test states may contain some of the
[assertions]({{ "/reference/assertions/" | relative_url}}) and some
of the [control functions]({{ "/reference/controls/" | relative_url }}).

[^1]: The initalization is copied into the initializer list of the generated
      test object. Perhaps, the framework will support the curly braces in
      the future.

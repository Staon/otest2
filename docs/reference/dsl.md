---
title: OTest2 DSL
excerpt: Description of the OTest2 DSL
permalink: /reference/dsl/
toc: true
---

## Basic Structure

The OTest2 files are valid C++ sources using simple macros and clang annotations
to describe the test. There are two flavors of the DSL: _compact syntax_
and _full syntax_. Both may be mixed together.

Test cases may contain zero or more test states. Suites may contain zero or
more test suites or test cases. Both test suite and test case may be
a root object (not placed in a parent).

Definition of a suite or a test case is divided into 3 blocks:
  1. Declaration of fixtures and user functions - **fixture block**.
  2. Definition of start-up, tear-down and user functions - **function block**.
  3. Definition of nested objects - **object block**.
     
There are no special marks dividing the blocks. First defined function
stops the fixture block and starts the function block. First nested
object stops the function block.

### Compact Syntax

The compact syntax is inherited from the original OTest project. The syntax
is more simple and it actually suites most use cases.
 
```c++
#include <otest2/otest2.h>

TEST_CASE(CaseName) {
  /* -- Fixture block of the case. Variables and declarations of user
   *    functions may be here. */
  int fixture(666);
  int myUserFunction();
  /* ... other fixtures ... */
  
  /* -- Function block of the suite. Maximally one start-up and one
   *    tear-down function may be defined. Number of user functions
   *    is not limited. */
  int myUserFunction() { /* ... */ }
  TEST_START_UP() { /* ... */ }
  TEST_TEAR_DOWN() { /* ... */ }
  /* ... other user functions ... */

  TEST_STATE(SecondState);              /* -- declaration */
  
  /* -- The test case can contain just test states. State marked
   *    by the macro TEST_SCENARIO is the entering state. If there
   *    is no scenario state, first implemented state is the entering
   *    one. */
  TEST_SCENARIO(FirstState) {
    /* -- The state may contain testing code. The scenario state
     *    may define test sections in addition. */
     
    TEST_SECTION(FirstSection) {
      /* ... */

      /* -- sections may be nested */
      TEST_SECTION(NestedSection) { /* ... */ }
      
      /* ... */
    }
    
    /* ... */
    
    TEST_SECTION(SecondSection) { /* ... */ }
    
    /* ... */
  }
    
  TEST_STATE(FirstState) { /* ... */ }
  
  TEST_STATE(SecondState) { /* ... */ } /* -- second state */
  
  /* ... other states ... */
  
  /* -- Following macro is a shortcut for cases which don't need
   *    to switch states - it creates one anonymous scenario state. */
  TEST_SIMPLE() { /* ... */ }
}

TEST_SUITE(SuiteName) {
  /* -- Fixture block of the suite. Variables and declarations of user
   *    functions may be here. */
  int fixture;
  int myUserFunction();
  /* ... other fixtures ... */
  
  /* -- Function block of the suite. Maximally one start-up and one
   *    tear-down function may be defined. The number of user functions
   *    is not limited. */
  int myUserFunction() { /* ... */ }
  TEST_START_UP() { /* ... */ }
  TEST_TEAR_DOWN() { /* ... */ }
  /* ... other user functions */
  
  /* -- Object block of the suite. Zero or more nested suites or
   *    test cases may be specified here. */
  /* ... children objects ... */
}
```

### Full Syntax

The full syntax is little bit more complex and it allows usage of advanced
features of the framework. Actually, the main difference is the possibility
to declare [user data]({{ "/examples/userdata/" | relative_url }}) as parameters
of [test functions](#functions).

```c++
#include <otest2/otest2.h>

OT2_CASE(CaseName) OT2_TAGS("...tags...") {
  /* -- Fixture block of the case. Variables and declarations of user
   *    functions may be here. */
  int fixture(666);
  int myUserFunction();
  /* ... other fixtures ... */
  
  /* -- Function block of the suite. Maximally one start-up and one
   *    tear-down function may be defined. Number of user functions
   *    is not limited. */
  int myUserFunction() { /* ... */ }
  void startUp() OT2_START_UP() { /* ... */ }
  void tearDown() OT2_TEAR_DOWN() { /* ... */ }
  /* ... other user functions ... */

  void secondState() OT2_STATE();              /* -- declaration */

  /* -- The test case can contain just test states. State marked
   *    by the macro TEST_SCENARIO is the entering state. If there
   *    is no scenario state, first implemented state is the entering
   *    one. */
  void firstState() OT2_SCENARIO() {
    /* -- The state may contain testing code. The scenario state
     *    may define test sections in addition. */
     
    OT2_SECTION(FirstSection) {
      /* ... */

      /* -- sections may be nested */
      OT2_SECTION(NestedSection) { /* ... */ }
      
      /* ... */
    }
    
    /* ... */
    
    OT2_SECTION(SecondSection) { /* ... */ }
    
    /* ... */
  }
  
  void secondState() OT2_STATE() { /* ... */ } /* -- second state */
  
  /* -- Following macro is a shortcut for cases which don't need
   *    to switch states - it creates one AnonymousState. */
  OT2_SIMPLE() { /* ... */ }
  
  /* ... other states ... */
}

OT2_SUITE(SuiteName) OT2_TAGS("...tags...") {
  /* -- Fixture block of the suite. Variables and declarations of user
   *    functions may be here. */
  int fixture;
  int myUserFunction();
  /* ... other fixtures ... */
  
  /* -- Function block of the suite. Maximally one start-up and one
   *    tear-down function may be defined. The number of user functions
   *    is not limited. */
  int myUserFunction() { /* ... */ }
  void startUp() OT2_START_UP() { /* ... */ }
  void tearDown() OT2_TEAR_DOWN() { /* ... */ }
  /* ... other user functions */
  
  /* -- Object block of the suite. Zero or more nested suites or
   *    test cases may be specified here. */
  /* ... children objects ... */
}
```

## Fixtures

Fixtures are just member variables which may be possibly initialized. **Just
initilizations by round braces (C++98 style) and by curly braces (C++11 style)**
are supported only. The C-style assignment initialization is not supported.

```c++
  std::string fixture1_;
  std::string fixture2_("Hello world");
  std::string fixture3_{"Hello world"};
```

A fixture may be annotated by the macro `OT2_USER_DATA` or `OT2_USER_DATA_KEY`.
Then it's handled as [user data]({{ "/examples/userdata/" | relative_url }}) -
the generator initializes the value from the user data container. 

See how to use the fixtures in the [example]({{ "/examples/fixtures/" | relative_url }}).

## Functions

There are 3 kinds of function handled in some special way:

  1. **start-up functions** - annotated by the macros `OT2_START_UP` or
     `TEST_START_UP`.
  2. **tear-down functions** - annotated by the macros `OT2_TEAR_DOWN` or
     `TEST_TEAR_DOWN`.
  3. **state functions** - annotated by the macros `OT2_STATE` or
     `TEST_STATE`.

The start-up function is invoked at the birth of the test object and
the tear-down function is invoked when the object is turned to death.
The state functions are invoked during execution of the test object.
Just function annotation matters, not the function name!

Functions of all three kinds must return `void`. Declared parameters are
interpreted as [user data]({{ "/examples/userdata/" | relative_url }}).
If the parameter is the type `const ::OTest2::Context&`
[the OTest2 Context]({{ "api/html/classOTest2_1_1Context.html" | relative_url }})
instance is passed into the parameter.

Every function with no annotation is handled as a **user function**. The function
is just copied into the generated source but it's wrapped by some mechanism
exporting it into nested test objects. So that's it, the test state
can call functions of the case and the suite, the case can call functions
of the suite. 

Body of a function is just ordinary C++ code. Except that the code can
invoke some of the [assertions]({{ "/reference/assertions/" | relative_url}})
or [control functions]({{ "/reference/controls/" | relative_url }}).

See how to use the start-up and tear-down methods in
the [example]({{ "/examples/fixtures/" | relative_url }}).
How to use the test states and user data is explained in the 
[custom main loop]({{ "/examples/main-loop/" | relative_url }})
and [user data]({{ "/examples/userdata/" | relative_url }}) examples. 

## Tags

Either suite or test case may be tagged by the directive `OT2_TAGS()`.
The parameter is a string literal containing name of the tags separated
by the comma. E.g.
```c++
OT2_TAGS("tag1,my-tag,otest.tag")
```
There are not allowed spaces around the commas. The tag name must match
the regular expression `[a-zA-Z_][a-zA-Z0-9_.\-]*`.

How to use tags is shown in [the tag example]({{ "/examples/tags/" | relative_url }}).
The glob language used for filtering of testing objects is described in
[the reference chapter]({{ "/reference/tagglob/" | relative_url }}).  

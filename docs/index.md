---
title: "About"
author_profile: true
sidebar: false
---

**OTest2** is a small C++ framework designated for writing of any kind of
automated tests (unit tests, integration tests, etc.). Its design is derived
from an older project of mine written while I was working in
[Aveco](http://www.aveco.com/) - that's why there is 2 in the name. As far as
I know the original project is still in use.

# Design Goals
The framework follows several design goals:
1. getting rid of boiler plate code,
2. being extensible for different development environments
3. and basic usability in IDEs.

The first goal is reached by a pre-processor based on the
[libclang](https://clang.llvm.org/docs/Tooling.html) which offers a full C++
parser. The OTest2 test descriptions are valid C++ sources extended by some
annotations (the annotations are hidden in macros). These files are parsed
by the pre-processor and transformed into  C++ sources extended by all needed
service code like filling filenames and line numbers into assertions,
enveloping of testing code into classes, registration of factory functions
of suites and cases or distribution of fixture variables between testing
objects.

In the sake of the second goal, the framework allows to make own implementations
of several crucial parts like custom assertions, catchers of unhandled
exceptions or test reporters. The framework can be easily integrated with your
custom main loop. Generally, it doesn't matter what is your development
environment. You can plug the framework in. More about extending of
the framework [here]({{ "/extending/" | relative_url }}).

The last goal means working IDE functions like code assistance with the test
DSL files. The test files are valid C++ files, so everything works instantly
with no needed effort.

The best way to explore the framework is following
[the examples]({{ "/examples/" | relative_url }}). More deep insight can be got
in the [reference documentation]({{ "/reference/" | relative_url }}). A guide how
to compile and install the framework in your environment can be found in
the [install guide]({{ "/install-guide/" | relative_url }}).

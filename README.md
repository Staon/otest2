# OTest2

**Project site:** [https://staon.github.io/otest2/](https://staon.github.io/otest2/)

**Current CI status:** [![Staon](https://circleci.com/gh/Staon/otest2.svg?style=svg)](https://circleci.com/gh/Staon/workflows/otest2/tree/master)

**Current version:** [1.0.0](https://github.com/Staon/otest2/releases/tag/v1.0.0)

**Downloads:** [https://github.com/Staon/otest2/releases/](https://github.com/Staon/otest2/releases/)

**OTest2** is a small C++ framework designated for writing of any kind of
automated tests (unit tests, integration tests, etc.). Its design is derived
from an older project of mine written while I was working for
[Aveco](http://www.aveco.com/) - that's why there is 2 in the name. As far as
I know the original project is still in use.

The project follows several design goals:

1. getting rid of writing boiler plate code,
2. extensibility of the framework for different development environments,
3. DSL compatible with IDE functions like syntax highlighting and code assist.

The framework offers features:

* a preprocessor generating service code.
* DSL is a valid C++ which doesn't break fancy IDE functions like code
  assistance.
* Grouping of test cases into test suites.
* A set of assertion functions. The user is allowed to implement custom
  assertions.
* Test fixtures at the level of either test case or test suite.
* Test fixture objects - objects automatically initialized and destroyed
  during the start-up and tear-down phase.
* Regression test marks - a text representation of inner state of some
  structure which can be stored and checked.
* Exception handling - the framework is able to check occurence of an
  exception and it checks unexpected exceptions fired from the test cases.
* Possible integration with custom main event loop.
* User data passed from the main function into the testing functions.
* Repeated runs of test cases or entire test suites.
* Nice color console output.
* Reporting in the JUnit XML file format.

See the project site [https://staon.github.io/otest2/](https://staon.github.io/otest2/)
for detailed documentation, examples and the install guide.

# OTest2

**Project site:** [https://staon.github.io/otest2/](https://staon.github.io/otest2/)

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

* a preprocessor generating service code 
* DSL is a valid C++ which doesn't fancy IDE functions like code assistance
* test cases are grouped into test suites
* a rich set of assertion functions with a possibility to create own one
* test fixtures at the level of either test case or test suite
* regression test marks
* exception handling (checking actual occurrence of an exception and handling
  of unexpected exceptions fired from a test case)
* nice colorized console output
* reporting in the JUnit XML file format
* possibile integration with custom main event loop

See the project site [https://staon.github.io/otest2/](https://staon.github.io/otest2/)
for detailed documentation, examples and the install guide.

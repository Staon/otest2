---
title: "About"
permalink: /
author_profile: true
circleci: true
toc: true
---

**OTest2** is a small C++ framework designated for writing of any kind of
automated tests (unit tests, integration tests, etc.). Its design is derived
from an older project of mine written while I was working for
[Aveco](http://www.aveco.com/) - that's why there is number 2 in the name.
As far as I know the original project is still in use.

Current release version is [1.2.0]({{ site.repositoryurl }}/releases/tag/v1.2.0).

[Github Repository]({{ site.repositoryurl }})

## Features

* An [xUnit](https://en.wikipedia.org/wiki/XUnit) kind framework.
* No need to write boiler plate code - it's done by a generator.
* DSL is a valid C++ code which doesn't break fancy IDE functions like code
  assistance.
* Very cautious usage of C macros.
* Test cases are grouped in suites.
* [Tags]({{ "/examples/tags/" | relative_url }}) can be assigned to both test
  suites and test cases. There is
  a [tag glob language]({{ "/reference/tagglob/" | relative_url }})
  allowing filtering of teting objects from test execution.
* A rich set of [assertion functions]({{ "/reference/assertions/" | relative_url }}).
  Beside the generic assertion there are relational assertions, assertions
  comparing content of STL containers or text files. The framework allows
  implementation of custom assertion functions.
* [Test fixtures]({{ "/examples/fixtures/" | relative_url }})
  are supported both in the suites or the test cases. The fixtures may be
  initialized and destroyed by their constructor and destructor methods
  (as it's common in C++) or they can be initialized in special start-up
  and tear-down methods.  
* User can implement
  [own fixture objects]({{ "/examples/fixture-objects/" | relative_url }})
  with hook methods invoked during start-up and tear-down phase of suite
  or test case where the object is used.
* Possible [integration with custom main event loop]({{ "/examples/main-loop/" | relative_url }}).
  The framework doesn't control run of the tests but it offers an API which
  can be invoked from the main loop. A default implementation of the loop
  is available too.
* [User data]({{ "/examples/userdata/" | relative_url }}) may be passed from
  the main function into the testing functions. User can inject some context
  into tests.
* [Regression test marks]({{ "/examples/regressions/" | relative_url }}) - 
  a structured representation of complex data like logical state of an object
  or some complex output of something. The marks can be stored and then checked
  in following runs of the test.
* [Exception handling]({{ "/examples/exceptions/" | relative_url }}). 
  The framework offers special try/catch directive which allows to check
  whether an expected exception occurs. Unexpected exceptions unwound from
  testing objects are caught, reported and the test is failed.
* A custom [exception catcher]({{ "/extending/catcher/" | relative_url }})
  can be injected into the framework to handle your exception objects. 
* One can implement a [repeater object]({{ "/examples/repeaters/" | relative_url }})
  which controls repeated run of testing objects. The repeater is not just
  a list of values but a dynamic object with possible access to fixture objects. 
* Optional report in the [JUnit](https://junit.org/junit5/) XML file format.
* Nice color console report:
 
{:refdef: style="text-align: center;"}
![color console report]({{ "/assets/console-reporter.png/" | relative_url }})
{:refdef}

### Supported Platforms

The framework is developed and tested on Debian Stretch distribution. Generally,
it should work on any UNIX-like system which the [libclang](https://clang.llvm.org/docs/Tooling.html)
is supported for. There is no current intention to support MS Windows.

## Design Goals

### No Boiler Plate Code

It's sometimes unbelievable how much work must be done in some testing frameworks
in order to write a two-lines test. The OTest2 framework allows you to
focus just on coding what you need. All the annoying work is done by
a pre-processor based on the [libclang](https://clang.llvm.org/docs/Tooling.html).

Libclang offers a full C++ parser opening door to a world of almost unlimited
possibilities. Hence, the OTest2 DSL has been implemented as a valid C++ source
marked by some clang annotations[^1]. The pre-processor parses a source file
and transforms it to another C++ source extended by all needed service code -
filling source locations into test assertions, enveloping of testing code
by classes, registration of factory functions of suites and cases or passing
of fixture variables between testing objects.

### Extensible for Different Environments[^2]

The framework offers several interfaces which can be implemented by the user
and injected into the framework. So it can be easily integrated with your
custom main event loop or a custom collector of test results. The user is
allowed to implement own test assertion, catcher of unhandled exceptions
or his very own test reporter.

More about extending is in the section
[Extending the Framework]({{ "/extending/" | relative_url }}).

### Usability in IDEs

There are some testing frameworks defining their special DSL. This is a mighty
approach allowing describing of everything needed. However, there is a dark side.
The code is not valid C++ and all fancy features of IDEs like code highlighting
or code assistance stop working until someone develops a plugin supporting
the DSL. 

OTest2 framework keeps that on its mind. The DSL is designed to be valid C++
code with cautious usage of macros[^3]. Hence, the OTest2 file should be fully
working in every sane IDE. 

## How to Explore

The best way to explore the framework is following
[the examples how to use]({{ "/examples/" | relative_url }}) and
[the examples how to extend]({{ "/extending/" | relative_url }}) the framework.
Reference style documentation can be to in
 the [reference documentation]({{ "/reference/" | relative_url }}). A guide how
to compile and install the framework in your environment can be found in
the [install guide]({{ "/install-guide/" | relative_url }}).

[^1]: The annotations are hidden in macros as their usage is not so easy
      as in other programming languages.

[^2]: The term _environment_ doesn't mean platform. It means different libraries
      and technologies, like custom main loop or some strange collector
      of test results.

[^3]: Some C++ testing frameworks wraps their functionality into really huge
      macros. Their definitions are so complex that the IDEs' code assistance
      parsers fail to process them.

---
title: Command Line Options
permalink: /reference/cmdline/
---

Following options are parsed by the class
[`DfltEnvironment`]({{ "api/html/classOTest2_1_1DfltEnvironment.html" | relative_url }}).

```plaintext
Options (all options are optional):
  -h       --help             Print this message.
           --disable-console  Disable reporting into the console.
  -v       --verbose          Make the console reporter verbose. This option
                              turns on printing of results of passed tests.
  -j file  --junit=file       Print report into a file in the JUnit XML format.
                              This option may be used several times. Several
                              files will then be written.
  -r obj   --restrictive=obj  Run just only the specified testing object.
                              'obj' is in the format '<suite>[::<case>]'.
                              If just the suite name is got the whole suite
                              is run. A complete object path means running
                              of just one test case.
  -m file  --regression=file  Path of the regression file. The default value
                              is 'regression.ot2tm' (stored in the working
                              directory).
  -t name  --test=name        Name of the test how it's reported. The default
                              value is the name of the test's binary.
```
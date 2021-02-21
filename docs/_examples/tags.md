---
title: Tagging of Testing Objects
lesson: 10
---
Tagging of test cases or suites and filtering of objects by set tags.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/tags)

Generally, the unit tests should be quick. However, integration tests usually
consume minutes or tens of minutes sometimes hours. As programmers run 
tests quite often they cannot wait such  long time. There must be a way
how to mark those tests and exclude them from run.

The OTest2 framework offers tags which may be assigned to either suite or case.
Let's see example:

```c++
TEST_SUITE(TagsSuite) {
  TEST_CASE(QuickCase) {
    TEST_SIMPLE() {
      testAssert(true);
    }
  }

  TEST_CASE(SlowCase) OT2_TAGS("slow") {
    TEST_SIMPLE() {
      sleep(20);
      testAssert(true);
    }
  }
}
```
It's obvious that the second test case consumes much time. We have tagged it
by the tag `slow`. Now if we run the test
```plaintext
$ ./tags 
 ================================= TagsSuite ==================================
  QuickCase                                                           [Passed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Passed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   1                   0                   1
  Cases                    1                   0                   1
  Checks                   1                   0                   1
  Errors                                                           0
  Test total                                                          [Passed]
 ==============================================================================
```
the second test case is not run. The default tag filter runs just untagged
objects. If we want to run all tests
the empty [tag glob]({{ "/reference/tagglob/" | relative_url }}) must be specified:
```plaintext
$ ./tags -r '' 
 ================================= TagsSuite ==================================
  QuickCase                                                           [Passed]
  SlowCase                                                            [Passed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Passed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   1                   0                   1
  Cases                    2                   0                   2
  Checks                   2                   0                   2
  Errors                                                           0
  Test total                                                          [Passed]
 ==============================================================================
```
Or we can ask just for tagged cases:
```plaintext
staon@otest2:~/programovani/otest2/examples/tags/build$ ./tags -r '**::#slow'
 ================================= TagsSuite ==================================
  SlowCase                                                            [Passed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Passed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   1                   0                   1
  Cases                    1                   0                   1
  Checks                   1                   0                   1
  Errors                                                           0
  Test total                                                          [Passed]
 ==============================================================================
```

For more information see the [tag reference]({{ "/reference/dsl/" | relative_url }}#tags)
or the [tag glob reference]({{ "/reference/tagglob/" | relative_url }}).
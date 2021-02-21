---
title: Test Sections
lesson: 3
---
An example showing usage of test sections in the _OTest2_ framework.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/sections)

Another approach to the job related with initialization of a testing environment
is offered by the test section feature. Let see following example:

```c++
TEST_CASE(TestSections) {
  TEST_SIMPLE() {
    std::cout << "Some initialization steps..." << std::endl;

    TEST_SECTION(FirstTestingSection) {
      std::cout << "First section" << std::endl;
    }

    TEST_SECTION(SecondTestingSection) {
      std::cout << "Second section" << std::endl;
    }

    std::cout << "Some cleaning up steps..." << std::endl;
  }
}
```

Testing sections force repeating the entire test case several times. Each
run of the case runs just once section. However, code out of the section
is run every time. So different sections may share the same initialization
code.

If the test above is run, similar output is got:
```plaintext
 ==============================================================================
Some initialization steps...
First section
Some cleaning up steps...
  TestSections (section: FirstTestingSection)                         [Passed]
Some initialization steps...
Second section
Some cleaning up steps...
  TestSections (section: SecondTestingSection)                        [Passed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   0                   0
  Cases                    2                   0                   2
  Checks                   0                   0                   0
  Errors                                                           0
  Test total                                                          [Passed]
 ==============================================================================
```
As you can see the test case is run twice for each section.

The sections may be nested and there may be some other initialization code
around nested sections. Then the test case is run once for each leaf section. 

---
title: Custom Assertion
lesson: 2
---
Implementation of a custom assertion function.

[Full source code of the example]({{ site.repositoryurl }}/blob/master/examples/assertion)

Imagine a situation when you want to compare two files. First one is generated
by a tested code, the second one is contains an expected output. To reach this
goal one can write helper function

```c++
bool compareFiles(
    std::istream& generated_,
    std::istream& expected_) {
  /* ...... */
}
```

and then use it in his test.

```c++
  testAssert(compareFiles(generated_, expected_));
```

If the assertion fails similar output will be shown

```plaintext
 ============================== CustomAssertion ===============================
[.../examples/assertion/filecompare.ot2:43] assertion::CustomAssertion::FileComp
are: 'compareFiles(current_, expected_)' has failed
  FileCompare                                                         [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    0                   1                   1
  Checks                   0                   1                   1
  Errors                                                           0
  Test total                                                          [Failed]
 ==============================================================================
```

The test correctly fails but no one knows what has realy happened. The OTest2
framework offers two ways how to solve this problem:
* the [AssertBean]({{ "api/html/classOTest2_1_1AssertBean.html" | relative_url }})
* implementation of custom assertion function.

The first option is a simple class containing a boolean flag and a message.
The `testAssert()` function is overloaded and if the flag is false the message
is reported.

The second option is more complex but it offers more possibilites.

Implementation of custom assertion consists of two parts:
* an implementation class,
* a declaration of the assertion.
Firstly, we'll create the implementation class.

```c++
#include <otest2/assertcontext.h>

class FileCompare : public AssertContext {
  public:
    /* -- inherit the constructor - the parent constructor is invoked
     *    from the generated code. */
    using AssertContext::AssertContext;

    bool testCompareFiles(
        std::istream& file_,
        std::istream& expected_);
};
```

The implementation class must derive from the `AssertContext` class and it **must**
inherit its constructor method. The `testCompareFiles` implements the comparison
logic which has been implemented in the `compareFiles` function.

```c++
bool FileCompare::testCompareFiles(
    std::istream& file_,
    std::istream& expected_) {
  /* -- read contents of the files */
  std::vector<std::string> current_data_;
  slurpFile(current_data_, file_);
  std::vector<std::string> expected_data_;
  slurpFile(expected_data_, expected_);

  /* -- compute the difference */
  DiffLogBlocks diff_log_;
  DiffLogBuilderBlock log_builder_(&diff_log_);
  hirschbergDiff(current_data_, expected_data_, log_builder_);

  bool result_(diff_log_.empty());
  if(result_) {
    /* -- There is no difference, the files match */
    enterAssertion(result_, "OK", false);
  }
  else {
    /* -- the files are different */
    enterAssertion(result_, "the file is different than the expected one", false);

    /* -- print the difference */
    for(const auto& difference_ : diff_log_) {
      for(int i_(difference_.left_begin); i_ < difference_.left_end; ++i_) {
        std::ostringstream oss_;
        oss_ << std::setfill('0') << std::setw(4) << (i_ + 1) << "     : + " << current_data_[i_];
        assertionMessage(result_, oss_.str());
      }

      for(int i_(difference_.right_begin); i_ < difference_.right_end; ++i_) {
        std::ostringstream oss_;
        oss_ << std::setfill('0') << "     " << std::setw(4) << (i_ + 1) << ": - " << expected_data_[i_];
        assertionMessage(result_, oss_.str());
      }
    }
  }
  return leaveAssertion(result_);
}
```

The function firstly reads contents of the files and then computes their
difference. If the difference is not empty the assertion fails and
the difference is reported.

Remark the functions `enterAssertion`, `assertionMessage` and `leaveAssertion`.
They are the interface of the `AssertContext` class for assertion implementation.
Every assertion **must** invoke once `enterAssertion` and `leaveAssertion` methods
in this order. And it can call zero or more times the `assertionMessage`
between them.

Now it's time to declare the assertion function.

```c++
#include <otest2/assertionannotation.h>

bool testFileCompare(
    std::istream& file_,
    std::istream& expected_) TEST_ASSERTION_MARK(::OTest2::Examples::FileCompare, testCompareFiles);
```

The assertion function is declared as any function. The only difference is
annotation `TEST_ASSERTION_MARK` which links the function to its implementation
class. One implementation class may define several assertion functions.

It's a good manner to return result flag from the assertion function (true
if the assertion passes). The OTest2 framework doesn't break running test
if an assertion fails. Hence, sometimes it's needed to take some checks
conditionaly:

```c++
  if(testAssertNotEqual(pointer_, nullptr)) {
    testAssertEqual(pointer_->getName(), "...");
  }
```

If the assertion function is a function template a special annotation
`TEST_ASSERTION_MARK_TEMPL` can be used. A part of the annotation may
be string `$n` which expands to a deduced type of the n-th template
argument. See how [the relational assertions]({{ "api/html/assertions_8h_source.html" | relative_url }})
are implemented.

Now if we change the test
```c++
TEST_SUITE(CustomAssertion) {
  TEST_CASE(FileCompare) {
    TEST_SIMPLE() {
      std::istringstream current_("Hello world");
      std::istringstream expected_("Hello world!\nHave a nice day.");
      testFileCompare(current_, expected_);
    }
  }
}
```
we'll get nice descriptive output:
```plaintext
 ============================== CustomAssertion ===============================
[.../otest2/examples/assertion/filecompare.ot2:41] assertion::CustomAssertion::F
ileCompare: the file is different than the expected one
    0001     : + Hello world
         0001: - Hello world!
         0002: - Have a nice day.
  FileCompare                                                         [Failed]
 ------------------------------------------------------------------------------
  Suite total                                                         [Failed]
 ================================ Test results ================================
                      Passed              Failed               Total
  Suites                   0                   1                   1
  Cases                    0                   1                   1
  Checks                   0                   1                   1
  Errors                                                           0
  Test total                                                          [Failed]
 ==============================================================================
```

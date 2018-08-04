#include "../include/otest2/otest2.h"

#include <iostream>
#include <memory>
#include <string>

namespace Test {

/**
 * @brief Example test suite
 */
TEST_SUITE(ExampleSuite) {
  std::string suite_text("foo");

  TEST_START_UP() {
    /* -- make the string upper */
    suite_text = "foo";

    std::cout << "StartUp" << std::endl;
  }

  TEST_CASE(FirstCase) {
    std::string (*(*case_var[10][20])())(std::string const * const);
    void (*case_fce)(int, const char* foo_);
    char (*(*fce2)())[10];
    const char* (* const(*fce3)())[10](nullptr);
    int std::string::* mptr;
    int (std::string::* mfce)(int);
    std::unique_ptr<std::string> temp1;

    TEST_START_UP() {

    }

    TEST_TEAR_DOWN() {

    }

    TEST_SIMPLE() {
      std::cout
          << suite_text << std::endl
          << case_var << std::endl
          << case_fce << std::endl
          << fce2 << std::endl
          << fce3 << std::endl;

      throw "ahoj";
    }
  }

  TEST_CASE(SecondCase) {
    int case_a_(1000);
    std::string case_b_("a");

    TEST_START_UP() {

    }

    TEST_STATE(FirstState) {
      int a_(10);
      int b_(a_ + 10);
      int c_(1010);

      testAssert(a_ == b_);
      testAssert(case_b_ == "a");

      testAssertEqual(a_, c_);
    }

    TEST_STATE(SecondState) {

    }
  }
}

TEST_SUITE(SecondSuite) {

}

} /* -- namespace Test */

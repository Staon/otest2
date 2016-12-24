#include <otest2/otest2.h>

#include <datstr/dstring.h>
#include <iostream>

char (*function())[10] {
  static char foo_[10];
  return  &foo_;
}

/**
 * @brief Example test suite
 */
TEST_SUITE(ExampleSuite) {
  dstring suite_text;
  TEST_CASE(   FirstCase ) {
    dstring (*(*case_var[10][20])())(dstring const * const);
    void (*case_fce)(int, const char* foo_);
    char (*(*fce2)())[10];
    TEST_STATE(Init) {
      std::cout << "Hello world" << std::endl;

      void (*foo)(int);
    }
  }
}

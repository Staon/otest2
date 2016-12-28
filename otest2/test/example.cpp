#include <otest2/otest2.h>

#include <datstr/dstring.h>
#include <iostream>

/**
 * @brief Example test suite
 */
TEST_SUITE(ExampleSuite) {
  dstring suite_text;
  TEST_CASE(   FirstCase ) {
    dstring (*(*case_var[10][20])())(dstring const * const);
    void (*case_fce)(int, const char* foo_);
    char (*(*fce2)())[10];
    const char* (* const(*fce3)())[10];
    TEST_STATE(Init) {
      std::cout << "Hello world" << std::endl;

      void (*foo)(int);
    }
  }
}

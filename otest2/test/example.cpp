#include <otest2/otest2.h>

#include <datstr/dstring.h>
#include <datstr/iptr.h>
#include <iostream>

/**
 * @brief Example test suite
 */
TEST_SUITE(ExampleSuite) {
  dstring suite_text;

  TEST_CTOR((suite_text("Ahoj"))) {
    /* -- make the string upper */
    suite_text.Toupper();
  }

  TEST_CASE(FirstCase) {
    dstring (*(*case_var[10][20])())(dstring const * const);
    void (*case_fce)(int, const char* foo_);
    char (*(*fce2)())[10];
    const char* (* const(*fce3)())[10];
    int dstring::* mptr;
    int (dstring::* mfce)(int);
    cIPtr<dstring> temp1;

    TEST_CTOR((case_fce(0), fce2(0), fce3(0), mptr(0), mfce(0))) {

    }

    TEST_DTOR() {

    }

    TEST_STATE(Init) {
      std::cout
          << suite_text << std::endl
          << case_var << std::endl
          << case_fce << std::endl
          << fce2 << std::endl
          << fce3 << std::endl;
    }
  }
}

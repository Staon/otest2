#include <iostream>
#include <string>

namespace OTest2 {

namespace Private {

struct EmptyFixtures {

};

} /* -- namespace Private */

} /* -- namespace OTest2 */

#define import , public

#define fixtures {

#define begin {

#define end }

#define ctor Fixtures() : FIXTURES_PARENT()

#define init ,

#define dtor ~Fixtures()

#define test_suite(name_) \
  struct name_; \
  struct name_##OTest2MidSuiteClass { \
    typedef name_ OTEST2_SUITE; \
    typedef ::OTest2::Private::EmptyFixtures FIXTURES_PARENT; \
  }; \
  struct name_ : public name_##OTest2MidSuiteClass { \
    struct Fixtures : public ::OTest2::Private::EmptyFixtures

#define test_case(name_) \
  struct name_; \
  struct name_##OTest2MidCaseClass { \
    typedef name_ OTEST2_CASE; \
    typedef OTEST2_SUITE::Fixtures FIXTURES_PARENT; \
  }; \
  struct name_ : public name_##OTest2MidCaseClass { \
    struct Fixtures : public OTEST2_SUITE::Fixtures

#define state_list };

#define test_state(name_) \
  struct name_; \
  struct name_##OTest2MidStateClass { \
    typedef name_ OTEST2_STATE; \
    typedef OTEST2_CASE::Fixtures FIXTURES_PARENT; \
  }; \
  struct name_ : public name_##OTest2MidStateClass, public OTEST2_CASE::Fixtures { \
    void otest2_case_method() {

#define init_state \
  test_state(otest2_init_state)

#define end_state }};

#define end_case };

#define case_list };

#define end_suite };


class Runtime {
  public:
    std::string runtime_string;

    explicit Runtime(
        const char* text_) :
      runtime_string(text_) {

    }
};

test_suite(MySuite)
  import Runtime
fixtures
  int a;
  std::string b;

  ctor
    init Runtime("Runtime, vole")
    init a(0)
    init b("Fixture, vole")
  begin
    b += ", kdybys nevedel.";
  end

  dtor
  begin
    b = "zruseno!";
    runtime_string = "Uz ho vickrat nepouzivej";
  end

case_list
  test_case(MyCase)
  fixtures
    std::string c;
  state_list
    init_state
      c.c_str();
      b.c_str();
      runtime_string.c_str();
    end_state

    test_state(CuedIn)
      b.append("foo");
    end_state
  end_case

  test_case(MyCase2)
  fixtures
  state_list
    init_state
      b.push_back(static_cast<char>(a));
      runtime_string.c_str();
      b += runtime_string;
    end_state
  end_case
end_suite

int main(
    int argc_,
    char* argv_[]) {
  return 0;
}

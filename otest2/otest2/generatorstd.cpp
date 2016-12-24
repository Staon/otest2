#include "generatorstd.h"

#include <datstr/dstring.h>
#include <iostream>
#include <ondrart/oassert.h>
#include <ondrart/odelete.h>

namespace OTest2 {

struct GeneratorStd::Impl {
  public:
    GeneratorStd* owner;
    std::ostream* output;

    /* -- current state */
    dstring suite;
    dstring testcase;
    dstring state;

  private:
    /* -- avoid copying */
    Impl(
        const Impl&);
    Impl& operator =(
        const Impl&);

  public:
    explicit Impl(
        GeneratorStd* owner_,
        std::ostream* output_);
    ~Impl();
};

GeneratorStd::Impl::Impl(
    GeneratorStd* owner_,
    std::ostream* output_) :
  owner(owner_),
  output(output_),
  suite(),
  testcase(),
  state() {

}

GeneratorStd::Impl::~Impl() {

}

GeneratorStd::GeneratorStd(
    std::ostream* output_) :
  pimpl(new Impl(this, output_)) {

}

GeneratorStd::~GeneratorStd() {

}

void GeneratorStd::dumpString(
    const dstring& string_) {
//  *pimpl -> output << "dump: \"" << string_ << '"' << std::endl;
  *pimpl -> output << string_;
}

void GeneratorStd::enterSuite(
    const dstring& suite_) {
  OASSERT_1(pimpl -> suite.IsNull() && pimpl -> testcase.IsNull() && pimpl -> state.IsNull());
  OASSERT_1(!suite_.IsNullEmpty());

  pimpl -> suite = suite_;

  *pimpl -> output
      << "class " << suite_ << " : public ::OTest2::SuiteGenerated {\n"
      << "  private:\n"
      << "    /* -- avoid copying */\n"
      << "    " << suite_ << "(\n"
      << "        const " << suite_ << "&);\n"
      << "    " << suite_ << "& operator =(\n"
      << "        const " << suite_ << "&);\n"
      << '\n'
      << "  private:\n";
}

void GeneratorStd::enterCase(
    const dstring& case_) {
  OASSERT_1(!pimpl -> suite.IsNull() && pimpl -> testcase.IsNull() && pimpl -> state.IsNull());
  OASSERT_1(!case_.IsNullEmpty());

  pimpl -> testcase = case_;

  *pimpl -> output
      << "    class " << case_ << " : public ::OTest2::CaseGenerated {\n"
      << "      private:\n"
      << "        /* -- avoid copying */\n"
      << "        " << case_ << "(\n"
      << "            const " << case_ << "&);\n"
      << "        " << case_ << "& operator =(\n"
      << "            const " << case_ << "&);\n"
      << '\n'
      << "      private:\n";
}

void GeneratorStd::enterState(
    const dstring& state_) {
  OASSERT_1(!pimpl -> suite.IsNull() && !pimpl -> testcase.IsNull() && pimpl -> state.IsNull());
  OASSERT_1(!state_.IsNullEmpty());

  pimpl -> state = state_;

  *pimpl -> output
      << "        class " << state_ << " : public ::OTest2::StateGenerated {\n"
      << "          private:\n"
      << "            /* -- avoid copying */\n"
      << "            " << state_ << "(\n"
      << "              const " << state_ << "&);\n"
      << "            " << state_ << "& operator =(\n"
      << "                const " << state_ << "&);\n"
      << "\n"
      << "          private:\n"
      << "            virtual void runState(\n"
      << "                const ::OTest2::Context& context_) {\n";
}

void GeneratorStd::leaveState() {
  OASSERT_1(!pimpl -> suite.IsNull() && !pimpl -> testcase.IsNull() && !pimpl -> state.IsNull());

  *pimpl -> output
      << "            }\n"
      << "\n"
      << "          public:\n"
      << "            explicit " << pimpl -> state << "(\n"
      << "                const ::OTest2::Context& context_,\n"
      << "                " << pimpl -> suite << "* suite_,\n"
      << "                " << pimpl -> testcase << "* case_) :\n"
      << "              ::OTest2::StateGenerated(context_), {\n"
      << "\n"
      << "            }\n"
      << "\n"
      << "            virtual ~" << pimpl -> state << "() {\n"
      << "\n"
      << "            }\n"
      << "        };\n";

  pimpl -> state.SetNull();
}

void GeneratorStd::leaveCase() {
  OASSERT_1(!pimpl -> suite.IsNull() && !pimpl -> testcase.IsNull() && pimpl -> state.IsNull());

  *pimpl -> output
      << "\n"
      << "      public:\n"
      << "        explicit " << pimpl -> testcase << "(\n"
      << "            const ::OTest2::Context& context_,\n"
      << "            " << pimpl -> suite << "* suite_) :\n"
      << "          ::OTest2::CaseGenerated(context_) {\n"
      << "\n"
      << "        }\n"
      << "\n"
      << "        virtual ~" << pimpl -> testcase << "() {\n"
      << "\n"
      << "        }\n"
      << "    };\n";

  pimpl -> testcase.SetNull();
}

void GeneratorStd::leaveSuite() {
  OASSERT_1(!pimpl -> suite.IsNull() && pimpl -> testcase.IsNull() && pimpl -> state.IsNull());

  *pimpl -> output
      << "\n"
      << "  public:\n"
      << "    explicit " << pimpl -> suite << "(\n"
      << "        const ::OTest2::Context& context_) :\n"
      << "      ::OTest2::SuiteGenerated(context_) {\n"
      << "\n"
      << "    }\n"
      << "\n"
      << "    virtual ~" << pimpl -> suite << "() {\n"
      << "\n"
      << "    }\n"
      << "};\n";

  pimpl -> suite.SetNull();
}

} /* -- namespace OTest2 */

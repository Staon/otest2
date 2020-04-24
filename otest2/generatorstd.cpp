/*
 * Copyright (C) 2018 Ondrej Starek
 *
 * This file is part of OTest2.
 *
 * OTest2 is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * OTest2 is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OTest2.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "generatorstd.h"

#include <assert.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "filereader.h"
#include "functions.h"
#include "generfmt.h"
#include "formatting.h"
#include "lcstream.h"
#include "vartable.h"

namespace OTest2 {

struct GeneratorStd::Impl {
  public:
    GeneratorStd* owner;

    /* -- reading and writing */
    LCStream output;
    FileReader* reader;

    /* -- testing domain which is used for registration of the suites */
    std::string domain;

    /* -- names of the input and output files as they are used in
     *    the #line directives. */
    std::string infile;
    std::string outfile;

    /* -- table of variables */
    VarTablePtr variables;

    /* -- fixture functions */
    FunctionsPtr fixtures;
    std::vector<FunctionPtr> start_up_fce;
    std::vector<FunctionPtr> tear_down_fce;

    /* -- state function */
    FunctionPtr state_fce;

    /* -- current indentation */
    int indent;

    /* -- current state */
    std::string suite;
    std::string testcase;
    std::string state;

    /* -- list of suites */
    typedef std::vector<std::string> Suites;
    Suites suites;

    /* -- list of cases */
    typedef std::vector<std::string> Cases;
    Cases cases;

    /* -- list of states */
    typedef std::vector<std::string> States;
    States states;

  private:
    /* -- avoid copying */
    Impl(
        const Impl&);
    Impl& operator =(
        const Impl&);

  public:
    explicit Impl(
        GeneratorStd* owner_,
        std::ostream* output_,
        FileReader* reader_,
        const std::string& domain_,
        const std::string& infile_,
        const std::string& outfile_);
    ~Impl();

    void writeUserLineDirective(
        const Location& begin_);
    void writeGenerLineDirective();
    void writeStateClass(
        const std::string& state_);
    void writeStateFactoryMethod(
        const std::string& state_);
};

GeneratorStd::Impl::Impl(
    GeneratorStd* owner_,
    std::ostream* output_,
    FileReader* reader_,
    const std::string& domain_,
    const std::string& infile_,
    const std::string& outfile_) :
  owner(owner_),
  output(output_),
  reader(reader_),
  domain(domain_),
  infile(infile_),
  outfile(outfile_),
  variables(),
  indent(0) {

}

GeneratorStd::Impl::~Impl() {

}

void GeneratorStd::Impl::writeUserLineDirective(
    const Location& begin_) {
  output << '\n';
  output << "#line " << begin_.getLine() << " ";
  writeCString(output, infile);
  output << "\n";
}

void GeneratorStd::Impl::writeGenerLineDirective() {
  output << '\n';
  output << "#line " << (output.getLineNo() + 1) << " ";
  writeCString(output, outfile);
  output << "\n";
}

void GeneratorStd::Impl::writeStateClass(
    const std::string& state_) {
  output
      << "\n\n"
      << "        class " << state_ << " : public ::OTest2::StateGenerated {\n"
      << "          private:\n";
  variables -> printDeclarations(output, indent);
  output
      << "\n"
      << "          public:\n"
      << "            /* -- avoid copying */\n"
      << "            " << state_ << "(\n"
      << "                const " << state_ << "&) = delete;\n"
      << "            " << state_ << "& operator =(\n"
      << "                const " << state_ << "&) = delete;\n"
      << "\n"
      << "            explicit " << state_ << "(\n"
      << "                const ::OTest2::Context& context_";
  variables->printParameters(output, indent + 2);
  output
      << ") :\n"
      << "              ::OTest2::StateGenerated(context_, \"" << state_ << "\")";
  variables->printInitializers(output, indent + 1);
  output
      << " {\n"
      << "\n"
      << "            }\n"
      << "\n"
      << "            virtual ~" << state_ << "() {\n"
      << "\n"
      << "            }\n"
      << "\n";
}

void GeneratorStd::Impl::writeStateFactoryMethod(
    const std::string& state_) {
  output
      << "        ::OTest2::StatePtr createState_" << state_ << "(\n"
      << "            const ::OTest2::Context& context_) {\n"
      << "          return ::OTest2::makePointer<" << state_ << ">(\n"
      << "              context_";
  variables->printArguments(output, indent + 3);
  output
      << ");\n"
      << "        }";
}

GeneratorStd::GeneratorStd(
    std::ostream* output_,
    FileReader* reader_,
    const std::string& domain_,
    const std::string& infile_,
    const std::string& outfile_) :
  pimpl(new Impl(this, output_, reader_, domain_, infile_, outfile_)) {

}

GeneratorStd::~GeneratorStd() {

}

void GeneratorStd::beginFile() {
  pimpl->output
      << "/*\n"
      << "  This file is generated by the otest2 preprocessor. Don't modify it!\n"
      << "*/\n"
      << '\n'
      << "#include <memory>\n"
      << "#include <string>\n"
      << "\n"
      << "#include <otest2/assertions.h>\n"
      << "#include <otest2/assertionsimpl.h>\n"
      << "#include <otest2/casegenerated.h>\n"
      << "#include <otest2/context.h>\n"
      << "#include <otest2/fcemarshaler.h>\n"
      << "#include <otest2/generutils.h>\n"
      << "#include <otest2/objectptr.h>\n"
      << "#include <otest2/registry.h>\n"
      << "#include <otest2/regressions.h>\n"
      << "#include <otest2/regressionsimpl.h>\n"
      << "#include <otest2/stategenerated.h>\n"
      << "#include <otest2/suitegenerated.h>\n"
      << "#include <otest2/userdata.h>\n"
      << '\n';
}

void GeneratorStd::startUserArea(
    const Location& begin_) {
  pimpl->writeUserLineDirective(begin_);
}

void GeneratorStd::copySource(
    const Location& begin_,
    const Location& end_) {
  pimpl->reader->writePart(pimpl->output, begin_, &end_);
}

void GeneratorStd::makeAssertion(
    const std::string& assertion_class_,
    const std::string& assertion_method_,
    const Location& begin_,
    const Location& end_,
    const Location& expr_end_) {
  pimpl->writeGenerLineDirective();

  /* -- make an instance of the assertion class initialized with the filename,
   *    line number and the checked expression. */
  pimpl->output << assertion_class_ << "(otest2Context(), ";
  writeCString(pimpl->output, pimpl->infile);
  pimpl->output << ", " << begin_.getLine() << ", ";
  std::string expression_(pimpl->reader->getPart(begin_, expr_end_));
  writeCString(pimpl->output, expression_);
  pimpl->output << ")";

  /* -- invoke the assertion method */
  std::string rest_args_(pimpl->reader->getPart(expr_end_, end_));
  pimpl->output << "." << assertion_method_ << "(";
  pimpl->writeUserLineDirective(begin_);
  pimpl->output << expression_ << rest_args_;
}

void GeneratorStd::makeStateSwitch(
    const Location& state_begin_,
    const Location& state_end_,
    const Location& delay_begin_,
    const Location& delay_end_) {
  pimpl->writeGenerLineDirective();

  /* -- invoke the switching function */
  pimpl->output << "switchState(otest2Context(), ";
  writeCString(pimpl->output, pimpl->reader->getPart(state_begin_, state_end_));
  pimpl->output << ", ";
  pimpl->reader->writePart(pimpl->output, delay_begin_, &delay_end_);
  pimpl->output << ")";

  pimpl->writeUserLineDirective(delay_end_);
}

void GeneratorStd::makeTryCatchBegin(
    const Location& begin_) {
  pimpl->output << "              ::OTest2::GenericAssertion(otest2Context(), ";
  writeCString(pimpl->output, pimpl->infile);
  pimpl->output << ", " << begin_.getLine() << ", \"\").testException(\n"
      << "                  [&]()->bool {\n"
      << "                    bool otest2_exception_happens_(false);\n"
      << "                    try {";
}

void GeneratorStd::makeCatchHandler(
    const std::string& type_,
    const std::string& varname_) {
  pimpl->output
      << "                    }\n"
      << "                    catch(" << type_
      << " " << varname_ << ") { otest2_exception_happens_ = true;";
}

void GeneratorStd::makeTryCatchEnd() {
  pimpl->output
      << "                    }\n"
      << "                return otest2_exception_happens_;\n"
      << "              });";
}

void GeneratorStd::endUserArea(
    const Location& end_) {
  pimpl->writeGenerLineDirective();
}

void GeneratorStd::enterSuite(
    const std::string& suite_) {
  assert(pimpl->suite.empty() && pimpl->testcase.empty() && pimpl->state.empty());
  assert(!suite_.empty());

  pimpl->suite = suite_;
  pimpl->variables = std::make_shared<VarTable>("suite_", nullptr);
  pimpl->fixtures = std::make_shared<Functions>(nullptr);
  pimpl->start_up_fce.emplace_back(nullptr);
  pimpl->tear_down_fce.emplace_back(nullptr);
  pimpl->suites.push_back(suite_);
  pimpl->indent += 2;

  pimpl->output
      << "class " << suite_ << " : public ::OTest2::SuiteGenerated {\n"
      << "  private:\n";
}

void GeneratorStd::finishSuiteFixtures() {
  assert(!pimpl -> suite.empty() && pimpl -> testcase.empty() && pimpl -> state.empty());

  /* -- suite variables */
  pimpl->variables->printDeclarations(pimpl->output, pimpl->indent);

  /* -- ctor and dtor */
  pimpl->output
      << "\n"
      << "  public:\n"
      << "    /* -- avoid copying */\n"
      << "    " << pimpl->suite << "(\n"
      << "        const " << pimpl->suite << "&) = delete;\n"
      << "    " << pimpl->suite << "& operator = (\n"
      << "        const " << pimpl->suite << "&) = delete;\n"
      << "\n"
      << "    explicit " << pimpl->suite << "(\n"
      << "        const ::OTest2::Context& context_) :\n"
      << "      ::OTest2::SuiteGenerated(context_, \"" << pimpl->suite << "\")";
  pimpl->variables->printInitializers(pimpl->output, pimpl->indent + 1);
  pimpl->output
      << " {\n"
      << "      registerAllCases();\n"
      << "      registerFixtures();\n"
      << "    }\n"
      << "\n"
      << "    virtual ~" << pimpl->suite << "() {\n"
      << "\n"
      << "    }\n"
      << "\n";
}

void GeneratorStd::finishSuiteFunctions() {
  assert(!pimpl -> suite.empty() && pimpl -> testcase.empty() && pimpl -> state.empty());
}

void GeneratorStd::enterCase(
    const std::string& case_) {
  assert(!pimpl -> suite.empty() && pimpl -> testcase.empty() && pimpl -> state.empty());
  assert(!case_.empty());

  pimpl->testcase = case_;
  pimpl->variables = std::make_shared<VarTable>("case_", pimpl -> variables);
  pimpl->fixtures = std::make_shared<Functions>(pimpl->fixtures);
  pimpl->start_up_fce.emplace_back(nullptr);
  pimpl->tear_down_fce.emplace_back(nullptr);
  pimpl->cases.push_back(case_);
  pimpl->indent += 2;

  pimpl->output
      << "\n\n"
      << "  private:\n"
      << "    class " << case_ << " : public ::OTest2::CaseGenerated {\n"
      << "      private:\n";
}

void GeneratorStd::finishCaseFixtures() {
  assert(!pimpl -> suite.empty() && !pimpl -> testcase.empty() && pimpl -> state.empty());

  /* -- suite variables */
  pimpl->variables->printDeclarations(pimpl->output, pimpl->indent);

  /* -- ctor and dtor */
  pimpl->output
      << "\n"
      << "      public:\n"
      << "        /* -- avoid copying */\n"
      << "        " << pimpl->testcase << "(\n"
      << "            const " << pimpl->testcase << "&) = delete;\n"
      << "        " << pimpl->testcase << "& operator = (\n"
      << "            const " << pimpl->testcase << "&) = delete;\n"
      << "\n"
      << "        explicit " << pimpl->testcase << "(\n"
      << "            const ::OTest2::Context& context_";
  pimpl->variables->printParameters(pimpl->output, pimpl->indent + 2);
  pimpl->output
      << ") :\n"
      << "          ::OTest2::CaseGenerated(context_, \"" << pimpl->testcase << "\")";
  pimpl->variables->printInitializers(pimpl->output, pimpl->indent + 1);
  pimpl->output
      << " {\n"
      << "          registerAllStates(context_);\n"
      << "          registerFixtures();\n"
      << "        }\n"
      << "\n"
      << "        virtual ~" << pimpl->testcase << "() {\n"
      << "\n"
      << "        }\n"
      << "\n";
}

void GeneratorStd::finishCaseFunctions() {
  assert(!pimpl -> suite.empty() && !pimpl -> testcase.empty() && pimpl -> state.empty());

}

void GeneratorStd::enterState(
    const std::string& state_,
    FunctionPtr state_fce_,
    const Location& fbegin_,
    const Location& fend_) {
  assert(!pimpl -> suite.empty() && !pimpl -> testcase.empty() && pimpl -> state.empty());
  assert(!state_.empty() && state_fce_ != nullptr);

  pimpl->state = state_;
  pimpl->variables = std::make_shared<VarTable>("state_", pimpl->variables);
  pimpl->states.push_back(state_);
  pimpl->indent += 2;
  pimpl->state_fce = state_fce_;

  pimpl->writeStateClass(state_);

  /* -- generate the function declaration */
  state_fce_->generateDeclaration(
      pimpl->output, pimpl->indent, "stateFunction");
}

void GeneratorStd::emptyState() {
  const std::string state_name_("AnonymousState");

  pimpl->variables = std::make_shared<VarTable>("state_", pimpl->variables);
  pimpl->states.push_back(state_name_);
  pimpl->indent += 2;

  pimpl->writeStateClass(state_name_);
  pimpl->output
      << "            virtual void runState(\n"
      << "                const Context& context_) {\n"
      << "\n"
      << "            }\n"
      << "        };\n\n";

  pimpl->indent -= 2;

  /* -- generate the factory method of the state */
  pimpl->writeStateFactoryMethod(state_name_);

  pimpl->variables = pimpl->variables->getPrevLevel();
}

void GeneratorStd::appendVariable(
    const std::string& name_,
    const std::string& type_) {
  pimpl->variables->appendVariable(name_, type_);
}

void GeneratorStd::appendVariableInit(
    const std::string& name_,
    const std::string& type_,
    const Location& ibegin_,
    const Location& iend_) {
  std::string initializer_(pimpl->reader->getPart(ibegin_, iend_));
  pimpl->variables->appendVariableWithInit(name_, type_, initializer_);
}

void GeneratorStd::appendUserData(
    const std::string& name_,
    const std::string& key_,
    const std::string& type_) {
  pimpl->variables->appendUserData(name_, key_, type_);
}

void GeneratorStd::appendStartUpFunction(
    FunctionPtr function_,
    const Location& fbegin_,
    const Location& fend_) {
  assert(function_ != nullptr);

  /* -- store the function to generate its marshaler */
  assert(pimpl->start_up_fce.back() == nullptr);
  pimpl->start_up_fce.back() = function_;

  /* -- generate the function declaration */
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->reader->writePart(pimpl->output, fbegin_, &fend_);
}

void GeneratorStd::appendTearDownFunction(
    FunctionPtr function_,
    const Location& fbegin_,
    const Location& fend_) {
  assert(function_ != nullptr);

  /* -- store the function to generate its marshaler */
  assert(pimpl->tear_down_fce.back() == nullptr);
  pimpl->tear_down_fce.back() = function_;

  /* -- generate the function declaration */
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->reader->writePart(pimpl->output, fbegin_, &fend_);
}

void GeneratorStd::appendGenericFunction(
    const Location& fbegin_,
    const Location& fend_,
    bool body_) {
  /* -- generate the function declaration */
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->reader->writePart(pimpl->output, fbegin_, &fend_);
  if(!body_)
    pimpl->output << ";\n";
}

void GeneratorStd::leaveState() {
  assert(!pimpl->suite.empty() && !pimpl->testcase.empty() && !pimpl->state.empty());

  /* -- generate marshaler of the state function */
  pimpl->output
      << "\n"
      << "            virtual void runState(\n"
      << "                const Context& context_) {\n";
  pimpl->state_fce->generateInvocation(
      pimpl->output, pimpl->indent + 1, "stateFunction");
  pimpl->output
      << "\n"
      << "            }\n";
  pimpl->output
      << "        };\n\n";

  pimpl->indent -= 2;

  /* -- generate the factory method of the state */
  pimpl->writeStateFactoryMethod(pimpl->state);

  pimpl->state.clear();
  pimpl->variables = pimpl->variables->getPrevLevel();
  pimpl->state_fce = nullptr;
}

void GeneratorStd::leaveCase() {
  assert(!pimpl->suite.empty() && !pimpl->testcase.empty() && pimpl->state.empty());

  /* -- generate registration of the states */
  pimpl->output
      << "\n\n"
      << "        void registerAllStates(\n"
      << "            const ::OTest2::Context& context_) {\n";
  for(const auto& state_ : pimpl->states) {
    pimpl->output
      << "          registerState(\n"
      << "              \"" << state_ << "\",\n"
      << "              createState_" << state_ << "(context_));\n";
  }
  pimpl->output
      << "        }\n"
      << "\n";

  /* -- add the case's start-up and tear-down functions */
  pimpl->fixtures->appendFixture(
      pimpl->start_up_fce.back(), pimpl->tear_down_fce.back());

  /* -- generate fixture marshalers */
  pimpl->fixtures->generateMarshalers(
      pimpl->output, pimpl->indent, pimpl->testcase);
  pimpl->output
      << "        void registerFixtures() {\n";
  pimpl->fixtures->generateRegistration(
      pimpl->output, pimpl->indent + 1, pimpl->testcase);
  pimpl->output
      << "        }\n";

  pimpl->output
      << "    };\n\n";

  pimpl->indent -= 2;

  /* -- generate the factory method of the case */
  pimpl->output
      << "    ::OTest2::CasePtr createCase_" << pimpl->testcase << "(\n"
      << "        const ::OTest2::Context& context_) {\n"
      << "      return ::OTest2::makePointer<" << pimpl->testcase << ">(\n"
      << "          context_";
  pimpl->variables->printArguments(pimpl->output, pimpl->indent + 3);
  pimpl->output
      << ");\n"
      << "    }";

  pimpl->testcase.clear();
  pimpl->variables = pimpl->variables->getPrevLevel();
  pimpl->fixtures = pimpl->fixtures->getPrevLevel();
  pimpl->start_up_fce.pop_back();
  pimpl->tear_down_fce.pop_back();
  pimpl->states.clear();
}

void GeneratorStd::leaveSuite() {
  assert(!pimpl->suite.empty() && pimpl->testcase.empty() && pimpl->state.empty());

  /* -- generate registrations of the test cases */
  pimpl->output
      << "\n\n"
      << "  private:\n"
      << "    void registerAllCases() {\n";
  for(const auto& case_ : pimpl->cases) {
    pimpl->output
      << "      registerCase(\n"
      << "          \"" << case_ << "\",\n"
      << "          std::make_shared< ::OTest2::CaseGeneratedFactory<" << pimpl->suite << ", " << case_ << "> >(\n"
      << "              this,\n"
      << "              &" << pimpl->suite << "::createCase_" << case_ << "));\n";
  }
  pimpl->output
      << "    }\n\n";

  /* -- add the suite's start-up and tear-down functions */
  pimpl->fixtures->appendFixture(
      pimpl->start_up_fce.back(), pimpl->tear_down_fce.back());

  /* -- generate fixture marshalers */
  pimpl->fixtures->generateMarshalers(
      pimpl->output, pimpl->indent, pimpl->suite);
  pimpl->output
      << "    void registerFixtures() {\n";
  pimpl->fixtures->generateRegistration(
      pimpl->output, pimpl->indent + 1, pimpl->suite);
  pimpl->output
      << "    }\n";

  pimpl->output
      << "};\n";

  pimpl->suite.clear();
  pimpl->variables = nullptr;
  pimpl->fixtures = nullptr;
  pimpl->start_up_fce.pop_back();
  pimpl->tear_down_fce.pop_back();
  pimpl->cases.clear();
  pimpl->indent -= 2;
}

void GeneratorStd::endFile(
    const Location& last_) {
  /* -- generate registration of suites */
  if(!pimpl->suites.empty()) {
    pimpl->output
        << "\n"
        << "namespace {\n"
        << "\n"
        << "class SuiteRegistrator {\n"
        << "  public:\n"
        << "    SuiteRegistrator() {\n";
    for(const std::string& suite_ : pimpl->suites) {
      pimpl->output
          << "      ::OTest2::Registry::instance(";
      writeCString(pimpl->output, pimpl->domain);
      pimpl->output
          << ").registerSuite(\n"
          << "          \"" << suite_ << "\",\n"
          << "          std::make_shared< ::OTest2::SuiteGeneratedFactory<" << suite_ << "> >());\n";
    }
    pimpl->output
        << "    }\n"
        << "} registrator_of_generated_suites;\n"
        << "\n"
        << "} /* -- namespace */\n";
  }

  pimpl->writeUserLineDirective(last_);
  pimpl->reader->writePart(pimpl->output, last_, nullptr);
}

} /* -- namespace OTest2 */

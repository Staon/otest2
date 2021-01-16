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
#include <otest2/utils.h>
#include <string>
#include <vector>

#include "filereader.h"
#include "functions.h"
#include "generfmt.h"
#include "formatting.h"
#include "lcstream.h"
#include "objectlist.h"
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

    /* -- repeaters */
    std::vector<std::string> repeater;

    /* -- state function */
    FunctionPtr state_fce;

    /* -- current indentation */
    int indent;

    /* -- path of the current object */
    std::vector<std::string> objectpath;

    /* -- list of children objects */
    std::vector<Parser::ObjectListPtr> objects;

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
    void writeObjectCtors(
        const std::string& parent_class_,
        const std::vector<std::string>& ctor_lines_);
    void writeStateFactoryMethod(
        const std::string& state_);
    void writeObjectFactoryMethod(
        const std::string& object_);
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
  repeater(),
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
  output << "\n\n";
  Formatting::printIndent(output, indent - 2);
  output << "class " << state_ << " : public ::OTest2::StateGenerated {\n";
  Formatting::printIndent(output, indent - 1);
  output << "private:\n";
  variables -> printDeclarations(output, indent);
  output << "\n";
  Formatting::printIndent(output, indent - 1);
  output << "public:\n";
  Formatting::printIndent(output, indent);
  output << "/* -- avoid copying */\n";
  Formatting::printIndent(output, indent);
  output << state_ << "(\n";
  Formatting::printIndent(output, indent + 2);
  output << "const " << state_ << "&) = delete;\n";
  Formatting::printIndent(output, indent);
  output << state_ << "& operator =(\n";
  Formatting::printIndent(output, indent + 2);
  output << "const " << state_ << "&) = delete;\n\n";
  output << "\n";
  Formatting::printIndent(output, indent);
  output << "explicit " << state_ << "(\n";
  Formatting::printIndent(output, indent + 2);
  output << "const ::OTest2::Context& context_";
  variables->printParameters(output, indent + 2);
  output << ") :\n";
  Formatting::printIndent(output, indent + 1);
  output << "::OTest2::StateGenerated(context_, \"" << state_ << "\")";
  variables->printInitializers(output, indent + 1);
  output << " {\n\n";
  Formatting::printIndent(output, indent);
  output << "}\n\n";
  Formatting::printIndent(output, indent);
  output << "virtual ~" << state_ << "() {\n\n";
  Formatting::printIndent(output, indent);
  output << "}\n\n";
}

void GeneratorStd::Impl::writeObjectCtors(
    const std::string& parent_class_,
    const std::vector<std::string>& ctor_lines_) {
  Formatting::printIndent(output, indent - 1);
  output << "public:\n";
  Formatting::printIndent(output, indent);
  output << "/* -- avoid copying */\n";
  Formatting::printIndent(output, indent);
  output << objectpath.back() << "(\n";
  Formatting::printIndent(output, indent + 2);
  output << "const " << objectpath.back() << "&) = delete;\n";
  Formatting::printIndent(output, indent);
  output << objectpath.back() << "& operator = (\n";
  Formatting::printIndent(output, indent + 2);
  output << "const " << objectpath.back() << "&) = delete;\n";
  output << "\n";
  Formatting::printIndent(output, indent);
  output << "explicit " << objectpath.back() << "(\n";
  Formatting::printIndent(output, indent + 2);
  output << "const ::OTest2::Context& context_";
  variables->printParameters(output, indent + 2);
  output << ") :\n";
  Formatting::printIndent(output, indent + 1);
  output << parent_class_ << "(context_, \"" << objectpath.back() << "\")";
  variables->printInitializers(output, indent + 1);
  output << " {\n";
  for(const auto& line_ : ctor_lines_) {
    Formatting::printIndent(output, indent + 1);
    output << line_ << "\n";
  }
  Formatting::printIndent(output, indent);
  output << "}\n\n";
  Formatting::printIndent(output, indent);
  output << "virtual ~" << objectpath.back() << "() {\n\n";
  Formatting::printIndent(output, indent);
  output << "}\n\n";
}

void GeneratorStd::Impl::writeStateFactoryMethod(
    const std::string& state_) {
  Formatting::printIndent(output, indent);
  output << "::OTest2::StatePtr createState_" << state_ << "(\n";
  Formatting::printIndent(output, indent + 2);
  output << "const ::OTest2::Context& context_) {\n";
  Formatting::printIndent(output, indent + 1);
  output << "return std::make_shared<" << state_ << ">(\n";
  Formatting::printIndent(output, indent + 3);
  output << "context_";
  variables->printArguments(output, indent + 3);
  output << ");\n";
  Formatting::printIndent(output, indent);
  output << "}";
}

void GeneratorStd::Impl::writeObjectFactoryMethod(
    const std::string& object_) {
  /* -- Generate the factory method of the object. The method is generated
   *    just for nested objects - root objects are created directly by
   *    the constructor method. */
  if(objects.size() > 1) { /* -- file itself */
    Formatting::printIndent(output, indent);
    output << "::OTest2::ObjectScenarioPtr create" << object_ << "_" << objectpath.back() << "(\n";
    Formatting::printIndent(output, indent + 2);
    output << "const ::OTest2::Context& context_";
    variables->printFactoryParameters(output, indent + 2);
    output << ") {\n";
    Formatting::printIndent(output, indent + 1);
    output << "return std::make_shared<" << objectpath.back() << ">(\n";
    Formatting::printIndent(output, indent + 3);
    output << "context_";
    variables->printArguments(output, indent + 3);
    output << ");\n";
    Formatting::printIndent(output, indent);
    output << "}";
  }
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
  /* -- initialize the list of testing objects */
  assert(pimpl->objects.empty());
  pimpl->objects.push_back(::OTest2::make_unique<Parser::ObjectList>());

  /* -- write heading of the generated source file */
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
      << "#include <otest2/assertionsitemwise.h>\n"
      << "#include <otest2/assertionsitemwiseimpl.h>\n"
      << "#include <otest2/assertionslexi.h>\n"
      << "#include <otest2/assertionslexiimpl.h>\n"
      << "#include <otest2/assertionsmap.h>\n"
      << "#include <otest2/assertionsmapimpl.h>\n"
      << "#include <otest2/assertionstext.h>\n"
      << "#include <otest2/casegenerated.h>\n"
      << "#include <otest2/context.h>\n"
      << "#include <otest2/fcemarshaler.h>\n"
      << "#include <otest2/generutils.h>\n"
      << "#include <otest2/objectptr.h>\n"
      << "#include <otest2/registry.h>\n"
      << "#include <otest2/regressions.h>\n"
      << "#include <otest2/regressionsimpl.h>\n"
      << "#include <otest2/scenariocase.h>\n"
      << "#include <otest2/scenariosuite.h>\n"
      << "#include <otest2/stategenerated.h>\n"
      << "#include <otest2/suitegenerated.h>\n"
      << "#include <otest2/tags.h>\n"
      << "#include <otest2/typetraits.h>\n"
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
    const std::vector<AssertionArg>& args_ranges_) {
  pimpl->writeGenerLineDirective();

  /* -- read all assertion parameters */
  std::vector<std::string> args_texts_;
  for(const auto& arg_ : args_ranges_) {
    args_texts_.push_back(pimpl->reader->getPart(arg_.begin, arg_.end));
  }

  /* -- make an instance of the assertion class initialized with the filename,
   *    line number and the list of stringifized arguments */
  pimpl->output << assertion_class_ << "(otest2Context(), ";
  writeCString(pimpl->output, pimpl->infile);
  pimpl->output << ", " << args_ranges_.front().begin.getLine() << ", {";
  bool comma_(false);
  for(const auto& arg_ : args_texts_) {
    if(comma_)
      pimpl->output << ", ";
    else
      comma_ = true;
    writeCString(pimpl->output, arg_);
  }
  pimpl->output << "})";

  /* -- invoke the assertion method */
  pimpl->output << "." << assertion_method_ << "(";
  pimpl->writeUserLineDirective(args_ranges_.front().begin);

  /* -- print the arguments */
  int line_(0);
  comma_ = false;
  for(int i_(0); i_ < args_ranges_.size(); ++i_) {
    const AssertionArg& range_(args_ranges_[i_]);

    /* -- print the comma after previous argument */
    if(comma_) {
      pimpl->output << ", ";

      /* -- insert new lines to synchronize the line numbers with the source */
      int nline_(range_.begin.getLine());
      while(line_ < nline_) {
        pimpl->output << '\n';
        ++line_;
      }
    }
    else
      comma_ = true;

    /* -- print the argument */
    pimpl->output << args_texts_[i_];

    /* -- store current line to allow synchronization with the source */
    line_ = range_.end.getLine();
  }
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
  Formatting::printIndent(pimpl->output, pimpl->indent + 1);
  pimpl->output << "::OTest2::GenericAssertion(otest2Context(), ";
  writeCString(pimpl->output, pimpl->infile);
  pimpl->output << ", " << begin_.getLine() << ", {}).testException(\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 3);
  pimpl->output << "[&]()->bool {\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 4);
  pimpl->output << "bool otest2_exception_happens_(false);\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 4);
  pimpl->output << "try {";
}

void GeneratorStd::makeCatchHandler(
    const std::string& type_,
    const std::string& varname_) {
  Formatting::printIndent(pimpl->output, pimpl->indent + 4);
  pimpl->output << "}\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 4);
  pimpl->output << "catch(" << type_
      << " " << varname_ << ") {\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 5);
  pimpl->output << "otest2_exception_happens_ = true;";
}

void GeneratorStd::makeTryCatchEnd() {
  Formatting::printIndent(pimpl->output, pimpl->indent + 4);
  pimpl->output << "}\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 4);
  pimpl->output << "return otest2_exception_happens_;\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 3);
  pimpl->output << "});";
}

void GeneratorStd::endUserArea(
    const Location& end_) {
  pimpl->writeGenerLineDirective();
}

void GeneratorStd::enterSuite(
    const std::string& suite_,
    const Parser::ObjectTags& tags_) {
  assert(!pimpl->objects.empty());
  assert(!suite_.empty());

  pimpl->objectpath.push_back(suite_);
  pimpl->objects.back()->appendSuite(suite_, tags_);
  pimpl->objects.push_back(::OTest2::make_unique<Parser::ObjectList>());
  pimpl->variables = std::make_shared<VarTable>("suite_", pimpl->variables);
  pimpl->fixtures = std::make_shared<Functions>(pimpl->fixtures);
  pimpl->start_up_fce.emplace_back(nullptr);
  pimpl->tear_down_fce.emplace_back(nullptr);
  pimpl->repeater.emplace_back("");
  pimpl->indent += 2;

  Formatting::printIndent(pimpl->output, pimpl->indent - 2);
  pimpl->output << "class " << suite_ << " : public ::OTest2::SuiteGenerated {\n";
  Formatting::printIndent(pimpl->output, pimpl->indent - 1);
  pimpl->output << "private:\n";
}

void GeneratorStd::finishSuiteFixtures() {
  assert(!pimpl->objectpath.empty());

  /* -- suite variables */
  pimpl->variables->printDeclarations(pimpl->output, pimpl->indent);
}

void GeneratorStd::finishSuiteFunctions() {
  assert(!pimpl->objectpath.empty());

  /* -- invokers of user functions */
  pimpl->variables->printInvokers(
      pimpl->output, pimpl->indent, pimpl->objectpath.back());
  pimpl->output << "\n";

  /* -- ctor and dtor */
  pimpl->writeObjectCtors("::OTest2::SuiteGenerated", {"registerFixtures();"});

  /* -- children are declared private */
  pimpl->output << "\n\n";
  Formatting::printIndent(pimpl->output, pimpl->indent - 1);
  pimpl->output << "private:\n";
}

void GeneratorStd::enterCase(
    const std::string& case_,
    const Parser::ObjectTags& tags_) {
  assert(!case_.empty());

  pimpl->objectpath.push_back(case_);
  pimpl->objects.back()->appendCase(case_, tags_);
  pimpl->variables = std::make_shared<VarTable>("case_", pimpl -> variables);
  pimpl->fixtures = std::make_shared<Functions>(pimpl->fixtures);
  pimpl->start_up_fce.emplace_back(nullptr);
  pimpl->tear_down_fce.emplace_back(nullptr);
  pimpl->repeater.emplace_back("");
  pimpl->indent += 2;

  /* -- begin the generated case class */
  Formatting::printIndent(pimpl->output, pimpl->indent - 2);
  pimpl->output << "class " << case_ << " : public ::OTest2::CaseGenerated {\n";
  Formatting::printIndent(pimpl->output, pimpl->indent - 1);
  pimpl->output << "private:\n";
}

void GeneratorStd::finishCaseFixtures() {
  assert(!pimpl->objectpath.empty());

  /* -- suite variables */
  pimpl->variables->printDeclarations(pimpl->output, pimpl->indent);
}

void GeneratorStd::finishCaseFunctions() {
  assert(!pimpl->objectpath.empty());

  /* -- invokers of user functions */
  pimpl->variables->printInvokers(
      pimpl->output, pimpl->indent, pimpl->objectpath.back());
  pimpl->output << "\n";

  /* -- ctor and dtor */
  pimpl->writeObjectCtors("::OTest2::CaseGenerated", {
      "registerAllStates(context_);",
      "registerFixtures();",
  });
}

void GeneratorStd::enterState(
    const std::string& state_,
    FunctionPtr state_fce_,
    const Location& fbegin_,
    const Location& fend_) {
  assert(!pimpl->objectpath.empty());
  assert(!state_.empty() && state_fce_ != nullptr);

  pimpl->objectpath.push_back(state_);
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
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "virtual void runState(\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 2);
  pimpl->output << "const Context& context_) {\n\n";
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "}\n";
  Formatting::printIndent(pimpl->output, pimpl->indent - 2);
  pimpl->output << "};\n\n";

  pimpl->indent -= 2;

  /* -- generate the factory method of the state */
  pimpl->writeStateFactoryMethod(state_name_);

  pimpl->variables = pimpl->variables->getPrevLevel();
}

void GeneratorStd::appendVariable(
    const std::string& name_,
    const std::string& type_,
    InitializerPtr initializer_) {
  if(initializer_ != nullptr)
    initializer_->materializeInitializer(*pimpl->reader);
  pimpl->variables->appendVariable(name_, type_, initializer_);
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
  pimpl->output << "\n";
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
  pimpl->output << "\n";
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->reader->writePart(pimpl->output, fbegin_, &fend_);
}

void GeneratorStd::appendFixtureObject(
    const std::string& name_,
    FunctionPtr start_up_,
    FunctionPtr tear_down_) {
  pimpl->fixtures->appendFixture(start_up_, tear_down_);
}

void GeneratorStd::appendGenericFunction(
    FunctionPtr function_,
    const Location& fbegin_,
    const Location& fend_) {
  /* -- insert the function into the table of variables to be passed into
   *    nested objects */
  pimpl->variables->appendUserFunction(function_);

  /* -- generate the function declaration */
  pimpl->output << "\n";
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->reader->writePart(pimpl->output, fbegin_, &fend_);
}

bool GeneratorStd::appendRepeater(
    const std::string& name_,
    const std::string& type_,
    InitializerPtr initializer_) {
  assert(!pimpl->repeater.empty() && !name_.empty() && !type_.empty());

  /* -- just one repeater in the testing object */
  if(!pimpl->repeater.back().empty())
    return false;

  pimpl->repeater.back() = type_;
  if(initializer_ != nullptr)
    initializer_->materializeInitializer(*pimpl->reader);
  pimpl->variables->appendRepeater(name_, type_, initializer_);

  return true;
}

void GeneratorStd::leaveState() {
  assert(!pimpl->objectpath.empty());

  /* -- generate marshaler of the state function */
  pimpl->output << "\n";
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "virtual void runState(\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 2);
  pimpl->output << "const ::OTest2::Context& context_) {\n";
  pimpl->state_fce->generateInvocation(
      pimpl->output, pimpl->indent + 1, "stateFunction");
  pimpl->output << "\n";
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "}\n";
  Formatting::printIndent(pimpl->output, pimpl->indent - 2);
  pimpl->output << "};\n\n";

  pimpl->indent -= 2;

  /* -- generate the factory method of the state */
  pimpl->writeStateFactoryMethod(pimpl->objectpath.back());

  pimpl->objectpath.pop_back();
  pimpl->variables = pimpl->variables->getPrevLevel();
  pimpl->state_fce = nullptr;
}

void GeneratorStd::leaveCase() {
  assert(!pimpl->objectpath.empty());

  /* -- generate registration of the states */
  pimpl->output << "\n\n";
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "void registerAllStates(\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 2);
  pimpl->output << "const ::OTest2::Context& context_) {\n";
  for(const auto& state_ : pimpl->states) {
    Formatting::printIndent(pimpl->output, pimpl->indent + 1);
    pimpl->output << "registerState(\n";
    Formatting::printIndent(pimpl->output, pimpl->indent + 3);
    pimpl->output << "\"" << state_ << "\",\n";
    Formatting::printIndent(pimpl->output, pimpl->indent + 3);
    pimpl->output << "createState_" << state_ << "(context_));\n";
  }
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "}\n\n";

  /* -- add the case's start-up and tear-down functions */
  pimpl->fixtures->prependFixture(
      pimpl->start_up_fce.back(), pimpl->tear_down_fce.back());

  /* -- generate fixture marshalers */
  pimpl->fixtures->generateMarshalers(
      pimpl->output, pimpl->indent, pimpl->objectpath.back());
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "void registerFixtures() {\n";
  pimpl->fixtures->generateRegistration(
      pimpl->output, pimpl->indent + 1, pimpl->objectpath.back());
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "}\n";
  Formatting::printIndent(pimpl->output, pimpl->indent - 2);
  pimpl->output << "};\n\n";

  pimpl->indent -= 2;

  /* -- generate the factory method of the test case */
  pimpl->writeObjectFactoryMethod("Case");

  /* -- store type of the repeater */
  pimpl->objects.back()->setRepeaterType(pimpl->repeater.back());

  pimpl->objectpath.pop_back();
  pimpl->variables = pimpl->variables->getPrevLevel();
  pimpl->fixtures = pimpl->fixtures->getPrevLevel();
  pimpl->start_up_fce.pop_back();
  pimpl->tear_down_fce.pop_back();
  pimpl->repeater.pop_back();
  pimpl->states.clear();
}

void GeneratorStd::leaveSuite() {
  assert(!pimpl->objectpath.empty() && !pimpl->objects.empty());

  /* -- generate registrations of the children (test cases and nested suites) */
  pimpl->output << "\n\n";
  Formatting::printIndent(pimpl->output, pimpl->indent - 1);
  pimpl->output << "public:\n";
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "static void registerAllChildren(\n";
  Formatting::printIndent(pimpl->output, pimpl->indent + 2);
  pimpl->output << "::OTest2::ScenarioContainerPtr parent_) {\n";
  pimpl->objects.back()->printRegistrationsInSuite(
      pimpl->output, pimpl->objectpath.back(), pimpl->indent + 1);
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "}\n\n";
  Formatting::printIndent(pimpl->output, pimpl->indent - 1);
  pimpl->output << "private:\n";

  /* -- add the suite's start-up and tear-down functions */
  pimpl->fixtures->prependFixture(
      pimpl->start_up_fce.back(), pimpl->tear_down_fce.back());

  /* -- generate fixture marshalers */
  pimpl->fixtures->generateMarshalers(
      pimpl->output, pimpl->indent, pimpl->objectpath.back());
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "void registerFixtures() {\n";
  pimpl->fixtures->generateRegistration(
      pimpl->output, pimpl->indent + 1, pimpl->objectpath.back());
  Formatting::printIndent(pimpl->output, pimpl->indent);
  pimpl->output << "}\n";

  Formatting::printIndent(pimpl->output, pimpl->indent - 2);
  pimpl->output << "};\n\n";

  pimpl->indent -= 2;

  /* -- generate the suite's factory method */
  pimpl->writeObjectFactoryMethod("Suite");

  /* -- store type of the repeater */
  pimpl->objects.pop_back();
  pimpl->objects.back()->setRepeaterType(pimpl->repeater.back());

  pimpl->objectpath.pop_back();
  pimpl->variables = pimpl->variables->getPrevLevel();
  pimpl->fixtures = pimpl->fixtures->getPrevLevel();
  pimpl->start_up_fce.pop_back();
  pimpl->tear_down_fce.pop_back();
  pimpl->repeater.pop_back();
}

void GeneratorStd::endFile(
    const Location& last_) {
  assert(!pimpl->objects.empty());

  /* -- generate registration of suites */
  if(!pimpl->objects.back()->isEmpty()) {
    pimpl->output << "\n";
    Formatting::printIndent(pimpl->output, pimpl->indent);
    pimpl->output << "namespace {\n\n";
    Formatting::printIndent(pimpl->output, pimpl->indent);
    pimpl->output << "class ObjectsRegistrator {\n";
    Formatting::printIndent(pimpl->output, pimpl->indent + 1);
    pimpl->output << "public:\n";
    Formatting::printIndent(pimpl->output, pimpl->indent + 2);
    pimpl->output << "ObjectsRegistrator() {\n";
    pimpl->objects.back()->printRegistrationsInFile(
        pimpl->output, pimpl->domain, pimpl->indent + 3);
    Formatting::printIndent(pimpl->output, pimpl->indent + 2);
    pimpl->output << "}\n";
    Formatting::printIndent(pimpl->output, pimpl->indent);
    pimpl->output << "} registrator_of_generated_test_objects;\n\n";
    Formatting::printIndent(pimpl->output, pimpl->indent);
    pimpl->output << "} /* -- namespace */\n";
  }

  pimpl->writeUserLineDirective(last_);
  pimpl->reader->writePart(pimpl->output, last_, nullptr);

  pimpl->objects.pop_back();
}

} /* -- namespace OTest2 */

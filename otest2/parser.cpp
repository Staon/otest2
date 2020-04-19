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

#include "parser.h"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <vector>

#include "excparser.h"
#include "filereader.h"
#include "generatorprinter.h"
#include "generatorstd.h"
#include "options.h"
#include <otest2/exc.h>
#include "parsercontext.h"
#include "parsersuite.h"

namespace OTest2 {

namespace Parser {

class ParserConsumer : public clang::ASTConsumer {
  private:
    ParserContext context;
    SuiteVisitor visitor;

  public:
    explicit ParserConsumer(
        Generator* generator_,
        bool* failure_,
        ParserException* exception_,
        clang::ASTContext* comp_context_,
        clang::SourceManager* srcmgr_,
        clang::LangOptions* langopts_);
    virtual ~ParserConsumer();

    void HandleTranslationUnit(
        clang::ASTContext& context_) final;
};

ParserConsumer::ParserConsumer(
    Generator* generator_,
    bool* failure_,
    ParserException* exception_,
    clang::ASTContext* comp_context_,
    clang::SourceManager* srcmgr_,
    clang::LangOptions* langopts_) :
  context(comp_context_, srcmgr_, langopts_, generator_, failure_, exception_),
  visitor(&context) {

}


ParserConsumer::~ParserConsumer() {

}

void ParserConsumer::HandleTranslationUnit(
    clang::ASTContext& context_) {
  /* -- opening braces of the file */
  context.generator->beginFile();

  /* -- traverse the tree */
  visitor.TraverseDecl(context_.getTranslationUnitDecl());

  /* -- close the file */
  if(!*context.failure)
    context.generator->endFile(context.last_location);
}

class ParserAction : public clang::ASTFrontendAction {
  private:
    Generator* generator;
    bool* failure;
    ParserException* exception;

  public:
    explicit ParserAction(
        Generator* generator_,
        bool* failure_,
        ParserException* exception_);
    virtual ~ParserAction();

    /* -- avoid copying */
    ParserAction(
        const ParserAction&) = delete;
    ParserAction& operator =(
        const ParserAction&) = delete;

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& ci_,
        clang::StringRef) final;
};

ParserAction::ParserAction(
    Generator* generator_,
    bool* failure_,
    ParserException* exception_) :
  generator(generator_),
  failure(failure_),
  exception(exception_) {

}

ParserAction::~ParserAction() {

}

std::unique_ptr<clang::ASTConsumer> ParserAction::CreateASTConsumer(
    clang::CompilerInstance& ci_,
    clang::StringRef strref_) {
  return std::make_unique<ParserConsumer>(
      generator,
      failure,
      exception,
      &ci_.getASTContext(),
      &ci_.getSourceManager(),
      &ci_.getLangOpts());
}

class FrontendFactory : public clang::tooling::FrontendActionFactory {
  private:
    Generator* generator;
    bool* failure;
    ParserException* exception;

  public:
    explicit FrontendFactory(
        Generator* generator_,
        bool* failure_,
        ParserException* exception_);
    virtual ~FrontendFactory();

    clang::FrontendAction* create();
};

FrontendFactory::FrontendFactory(
    Generator* generator_,
    bool* failure_,
    ParserException* exception_) :
  generator(generator_),
  failure(failure_),
  exception(exception_) {

}

FrontendFactory::~FrontendFactory() {

}

clang::FrontendAction* FrontendFactory::create() {
  return new ParserAction(generator, failure, exception);
}

class ParserFailure : public Exception {
  public:
    /**
     * @brief Ctor
     */
    ParserFailure();

    /**
     * @brief Move ctor
     */
    ParserFailure(
        ParserFailure&& exc_);

    /**
     * @brief Dtor
     */
    virtual ~ParserFailure();

    /* -- avoid copying */
    ParserFailure(
        const ParserFailure&) = delete;
    ParserFailure& operator =(
        const ParserFailure&) = delete;

    /* -- exception interface */
    virtual std::string reason() const;
};

ParserFailure::ParserFailure() {

}

ParserFailure::ParserFailure(
    ParserFailure&& exc_) :
  Exception(std::move(exc_)) {

}

ParserFailure::~ParserFailure() {

}

std::string ParserFailure::reason() const {
  return "the parser failed";
}

} /* -- namespace Parser */

/* -- command line options */
llvm::cl::OptionCategory ParserOptCategory("otest2 options");
llvm::cl::extrahelp CommonHelp(
    clang::tooling::CommonOptionsParser::HelpMessage);
llvm::cl::extrahelp MoreHelp("\nOTest2 preprocessor");

void parse(
    const Options& options_) {
  /* -- prepare the options */
  std::vector<std::string> opts_;
  options_.fillClangToolOptions(opts_);
  std::vector<const char*> argv_;
  for(const auto& arg_ : opts_)
    argv_.push_back(arg_.c_str());

  /* -- prepare the reader and the output generator */
  std::ifstream ifs_(options_.getInfile());
  FileReader reader_(&ifs_);
  std::ostream *os_(nullptr);
  std::ofstream ofs_;
  if(options_.getOutfile() == "-") {
    os_ = &std::cout;
  }
  else {
    ofs_.open(options_.getOutfile());
    os_ = &ofs_;
  }
  GeneratorStd generator_(
      os_,
      &reader_,
      options_.getDomain(),
      options_.getInfile(),
      options_.getOutfile());
//  GeneratorPrinter generator_;

  /* -- parse the file */
  int argc_(argv_.size());
  clang::tooling::CommonOptionsParser parseropts_(
      argc_, argv_.data(), ParserOptCategory);
  clang::tooling::ClangTool tool_(
      parseropts_.getCompilations(),
      parseropts_.getSourcePathList());
  bool failure_(false);
  ParserException exception_;
  int retval_(
      tool_.run(new Parser::FrontendFactory(&generator_, &failure_, &exception_)));
  if(retval_ != 0) {
    /* -- remove the half created file */
    if(options_.getOutfile() != "-")
      unlink(options_.getOutfile().c_str());

    throw Parser::ParserFailure();
  }

  /* -- report an error */
  if(failure_)
    throw ParserException(std::move(exception_));
}

} /* -- namespace OTest2 */

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
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <fstream>
#include <iostream>
#include <llvm/Support/raw_ostream.h>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "error.h"
#include <exc.h>
#include "excparser.h"
#include "filereader.h"
#include "generatorprinter.h"
#include "generatorstd.h"
#include "location.h"
#include "options.h"
#include "typetemplate.h"

namespace OTest2 {

const std::string SUITE_ANNOTATION("otest2::suite");
const std::string CASE_ANNOTATION("otest2::case");
const std::string STATE_ANNOTATION("otest2::state");
const std::string START_UP_ANNOTATION("otest2::startUp");
const std::string TEAR_DOWN_ANNOTATION("otest2::tearDown");

const std::string ASSERTION_ANNOTATION("^otest2::assertion[(]([^,]+([,][^,]+)*)[)]$");

Location createLocation(
    clang::SourceManager* srcmgr_,
    const clang::SourceLocation& loc_) {
  clang::PresumedLoc ploc_(srcmgr_->getPresumedLoc(loc_));
  return Location(ploc_.getLine(), ploc_.getColumn());
}

template<typename Compare_>
bool hasAnnotation(
    clang::Decl* decl_,
    Compare_& cmp_) {
  if(decl_->hasAttrs()) {
    for(
        auto iter_(decl_->attr_begin());
        iter_ != decl_->attr_end();
        ++iter_) {
      if((*iter_)->getKind() == clang::attr::Annotate) {
        auto sa_(clang::cast<clang::AnnotateAttr>(*iter_));
        auto str_(sa_->getAnnotation());
        if(cmp_(str_.str())) {
          return true;
        }
      }
    }
  }
  return false;
}

struct AnnotationStringEqual {
    std::string ref;
    bool operator()(
        const std::string& annotation_) {
      return annotation_ == ref;
    }
};

struct AnnotationRegex {
    std::regex expr;
    std::smatch matches;
    AnnotationRegex(
        const std::string& expr_) :
      expr(expr_) {

    }
    bool operator()(
        const std::string& annotation_) {
      return std::regex_match(annotation_, matches, expr);
    }
};

bool hasAnnotation(
    clang::Decl* decl_,
    const std::string& annotation_) {
  AnnotationStringEqual cmp_{annotation_};
  return hasAnnotation(decl_, cmp_);
}

template<typename Node_>
clang::SourceRange getNodeRange(
    clang::SourceManager* srcmgr_,
    clang::LangOptions* langopts_,
    Node_* node_) {
  clang::SourceRange range_(node_->getSourceRange());

  /* -- adjust range at the end of last token */
  clang::SourceLocation true_end_(
      clang::Lexer::getLocForEndOfToken(
          range_.getEnd(),
          0,
          *srcmgr_,
          *langopts_));

  return clang::SourceRange(range_.getBegin(), true_end_);
}

struct ParserContext {
    /* -- error reporting */
    bool* failure;
    ParserException* exception;

    /* -- parser state */
    enum State {
      ROOT = 0,
      SUITE_BEGIN,
      SUITE_TEAR_DOWN,
      SUITE_CASES,
      CASE_BEGIN,
      CASE_TEAR_DOWN,
      CASE_STATES,
    };
    State state;
    clang::ASTContext* comp_context;
    clang::SourceManager* srcmgr;
    clang::LangOptions* langopts;

    /* -- output generation */
    Generator* generator;
    Location last_location;

    explicit ParserContext(
        clang::ASTContext* comp_context_,
        clang::SourceManager* srcmgr_,
        clang::LangOptions* langopts_,
        Generator* generator_,
        bool* failure_,
        ParserException* exception_);

    template<typename Node_>
    void setError(
        const std::string& message_,
        Node_* node_);
    template<typename Node_>
    void copyInput(
        Node_* node_,
        bool end_);
    template<typename Node_>
    void moveToEnd(
        Node_* node_);
};

ParserContext::ParserContext(
    clang::ASTContext* comp_context_,
    clang::SourceManager* srcmgr_,
    clang::LangOptions* langopts_,
    Generator* generator_,
    bool* failure_,
    ParserException* exception_) :
  failure(failure_),
  exception(exception_),
  state(ROOT),
  comp_context(comp_context_),
  srcmgr(srcmgr_),
  langopts(langopts_),
  generator(generator_),
  last_location() {

}

template<typename Node_>
void ParserContext::setError(
    const std::string& message_,
    Node_* node_) {
  /* -- get error location */
  clang::SourceLocation beg_(node_->getLocStart());
  clang::SourceLocation end_(node_->getLocEnd());
  clang::PresumedLoc pbeg_(srcmgr->getPresumedLoc(beg_));
  clang::PresumedLoc pend_(srcmgr->getPresumedLoc(end_));

  /* -- fill the exception */
  *failure = true;
  exception->setException(
      message_,
      pbeg_.getFilename(),
      pbeg_.getLine(),
      pbeg_.getColumn(),
      pend_.getLine(),
      pend_.getColumn());
}

template<typename Node_>
void ParserContext::copyInput(
    Node_* node_,
    bool end_) {
  /* -- get node's location */
  clang::SourceRange range_(getNodeRange(srcmgr, langopts, node_));
  clang::SourceLocation loc_;
  if(!end_)
    loc_ = range_.getBegin();
  else
    loc_ = range_.getEnd();
  Location l2_(createLocation(srcmgr, loc_));

  /* -- get content of the input file */
  if(last_location < l2_) {
    generator->startUserArea(last_location);
    generator->copySource(last_location, l2_);
    generator->endUserArea(l2_);
  }
}

template<typename Node_>
void ParserContext::moveToEnd(
    Node_* node_) {
  clang::SourceRange range_(getNodeRange(srcmgr, langopts, node_));
  last_location = createLocation(srcmgr, range_.getEnd());
}

class AssertVisitor : public clang::RecursiveASTVisitor<AssertVisitor> {
  private:
    ParserContext* context;
    Location current;

  public:
    explicit AssertVisitor(
        ParserContext* context_,
        const Location& curr_);
    virtual ~AssertVisitor();

    bool VisitCallExpr(
        clang::CallExpr* expr_);

    Location getCurrentLocation() const;
};

AssertVisitor::AssertVisitor(
    ParserContext* context_,
    const Location& curr_) :
  context(context_),
  current(curr_) {

}

AssertVisitor::~AssertVisitor() {

}

bool AssertVisitor::VisitCallExpr(
    clang::CallExpr* expr_) {
  /* -- Any invocation of a function can be calling of an assertion.
   *    I will get the function declaration. If the function is annotated
   *    being an assertion, I'll process it. */
  auto callee_(expr_->getCallee());
  if(!clang::isa<clang::ImplicitCastExpr>(callee_))
    return true;
  auto cast_(clang::cast<clang::ImplicitCastExpr>(callee_));
  auto declref_(cast_->getReferencedDeclOfCallee());
  if(declref_ == nullptr)
    return true;
//  This might be helpful in the future.
//  if(clang::isa<clang::CXXMethodDecl>(declref_)) {
//    auto method_(clang::cast<clang::CXXMethodDecl>(declref_));
//    return true;
//  }
  if(!clang::isa<clang::FunctionDecl>(declref_))
    return true;

  /* -- check the function annotation */
  AnnotationRegex cmp_(ASSERTION_ANNOTATION);
  if(!hasAnnotation(declref_, cmp_))
    return true;

  /* -- split the annotation arguments */
  auto annot_args_(cmp_.matches[1].str());
  std::regex split_("(\\s*;\\s*)");
  std::sregex_token_iterator iter_(
      annot_args_.begin(), annot_args_.end(), split_, -1);
  std::vector<std::string> annotation_args_(iter_, std::sregex_token_iterator());
  if(annotation_args_.size() != 2) {
    context->setError("invalid count of the arguments of the assertion annotation", declref_);
    return false;
  }

  /* -- parse name of the assertion */
  auto fce_(clang::cast<clang::FunctionDecl>(declref_));
  auto fcename_(fce_->getName().str());

  /* -- if the function is a template function, expand deduced types into
   *    the annotation parameters. */
  if(fce_->isTemplateInstantiation()) {
    const clang::TemplateArgumentList* args_(fce_->getTemplateSpecializationArgs());
    if(args_ != nullptr) {
      std::vector<std::string> template_params_;
      for(int i_(0); i_ < static_cast<int>(args_->size()); ++i_) {
        auto arg_(args_->get(i_));
        if(arg_.getKind() == clang::TemplateArgument::Type) {
          auto type_(arg_.getAsType());
          template_params_.push_back(type_.getAsString());
        }
      }
      if(!expandTemplates(annotation_args_, template_params_)) {
        context->setError("invalid type template in the annotation of the assertion", declref_);
        return false;
      }
    }
  }

  /* -- find first and last non-default argument */
  const int argnum_(expr_->getNumArgs());
  int last_index_(-1);
  for(int i_(0); i_ < argnum_; ++i_) {
    clang::Expr *arg_(expr_->getArg(i_));
    if(clang::isa<clang::CXXDefaultArgExpr>(arg_))
      break;
    last_index_ = i_;
  }
  if(last_index_ < 0) {
    context->setError("there are no arguments of the assertion", fce_);
    return false;
  }

  /* -- copy source just before the assertion */
  clang::SourceRange expr_range_(
      getNodeRange(context->srcmgr, context->langopts, expr_));
  auto expr_begin_(createLocation(context->srcmgr, expr_range_.getBegin()));
  context->generator->copySource(current, expr_begin_);

  /* -- Generate the assertion. I need to know the range of arguments
   *    because I want to insert the first argument as a text (to be shown
   *    in the assertion message. */
  auto begarg_(expr_->getArg(0));
  auto endarg_(expr_->getArg(last_index_));
  clang::SourceRange begrange_(
      getNodeRange(context->srcmgr, context->langopts, begarg_));
  clang::SourceRange endrange_(
      getNodeRange(context->srcmgr, context->langopts, endarg_));
  auto args_begin_(createLocation(context->srcmgr, begrange_.getBegin()));
  auto args_end_(createLocation(context->srcmgr, endrange_.getEnd()));
  auto msg_end_(createLocation(context->srcmgr, begrange_.getEnd()));
  context->generator->makeAssertion(
      annotation_args_[0],
      annotation_args_[1],
      args_begin_,
      args_end_,
      msg_end_);

  /* -- keep last position for next source copying */
  current = args_end_;

  return true;
}

Location AssertVisitor::getCurrentLocation() const {
  return current;
}

class SuiteVisitor : public clang::RecursiveASTVisitor<SuiteVisitor> {
  private:
    ParserContext* context;

    clang::VarDecl* getVarDecl(
        clang::Stmt* stmt_);
    clang::Stmt* getFunctionBody(
        clang::FunctionDecl* fce_);

    bool parseVariable(
        clang::VarDecl* vardecl_);
    bool parseCodeBlock(
        clang::Stmt* stmt_);
    bool parseCaseBody(
        clang::NamespaceDecl* ns_);
    bool parseSuiteBody(
        clang::NamespaceDecl* ns_);
    bool parseSuite(
        clang::NamespaceDecl* ns_);

  public:
    explicit SuiteVisitor(
        ParserContext* context_);
    virtual ~SuiteVisitor();

    bool VisitNamespaceDecl(
        clang::NamespaceDecl* ns_);
};

SuiteVisitor::SuiteVisitor(
    ParserContext* context_) :
  context(context_) {

}

SuiteVisitor::~SuiteVisitor() {

}

clang::VarDecl* SuiteVisitor::getVarDecl(
    clang::Stmt* stmt_) {
  /* -- check the declaration statement */
  if(!clang::isa<clang::DeclStmt>(stmt_)) {
    context->setError("invalid declaration statement", stmt_);
    return nullptr;
  }

  /* -- there must be exactly one variable */
  clang::DeclStmt* declstmt_(clang::cast<clang::DeclStmt>(stmt_));
  if(declstmt_->decl_begin() == declstmt_->decl_end()
     || !clang::isa<clang::VarDecl>(*declstmt_->decl_begin())
     || (declstmt_->decl_begin() + 1) != declstmt_->decl_end()) {
    context->setError("invalid declaration statement", declstmt_);
    return nullptr;
  }
  return clang::cast<clang::VarDecl>(*declstmt_->decl_begin());
}

clang::Stmt* SuiteVisitor::getFunctionBody(
    clang::FunctionDecl* fce_) {
  if(!fce_->hasBody()) {
    context->setError("function doesn't have a body", fce_);
    return nullptr;
  }
  return fce_->getBody();
}

bool SuiteVisitor::parseVariable(
    clang::VarDecl* vardecl_) {
  std::string varname_(vardecl_->getNameAsString());
  std::string type_(vardecl_->getType().getAsString());

  clang::Expr* init_(vardecl_->getInit());
  if(init_ == nullptr) {
    /* -- no initializer */
    context->generator->appendVariable(varname_, type_);
  }
  else {
    /* -- variable with initializer */
    if(vardecl_->getInitStyle() != clang::VarDecl::CallInit) {
      context->setError("only the callinit is supported", vardecl_);
      return false;
    }

    /* -- expression with cleanups is a wrapper, I nest into it. */
    if(clang::isa<clang::ExprWithCleanups>(init_)) {
      init_ = clang::cast<clang::ExprWithCleanups>(init_)->getSubExpr();
    }

    /* -- get range of the expression */
    clang::SourceRange range_(
        getNodeRange(context->srcmgr, context->langopts, init_));

//    std::cout << init_->getStmtClassName() << std::endl;

    /* -- adjust the range for constructor call */
    if(clang::isa<clang::CXXConstructExpr>(init_)) {
      clang::CXXConstructExpr* ctrexpr_(
          clang::cast<clang::CXXConstructExpr>(init_));
      const int argnum_(ctrexpr_->getNumArgs());

      /* -- find first and last non-default argument */
      int last_index_(-1);
      if(argnum_ > 0) {
        for(int i_(0); i_ < argnum_; ++i_) {
          clang::Expr *arg_(ctrexpr_->getArg(i_));
          if(clang::isa<clang::CXXDefaultArgExpr>(arg_))
            break;
          last_index_ = i_;
        }
      }

      /* -- get source range of the arguments */
      if(last_index_ < 0) {
        range_ = clang::SourceRange(range_.getEnd(), range_.getEnd());
      }
      else {
        clang::SourceRange first_(
            getNodeRange(
                context->srcmgr,
                context->langopts,
                ctrexpr_->getArg(0)));
        clang::SourceRange last_(
            getNodeRange(
                context->srcmgr,
                context->langopts,
                ctrexpr_->getArg(last_index_)));
        range_ = clang::SourceRange(first_.getBegin(), last_.getEnd());
      }
    }

    /* -- notify the generator */
    context->generator->appendVariableInit(
        varname_,
        type_,
        createLocation(context->srcmgr, range_.getBegin()),
        createLocation(context->srcmgr, range_.getEnd()));
  }

  return true;
}

bool SuiteVisitor::parseCodeBlock(
    clang::Stmt* stmt_) {
  /* -- get source range of the block */
  clang::SourceRange range_(
      getNodeRange(context->srcmgr, context->langopts, stmt_));
  Location begin_(createLocation(context->srcmgr, range_.getBegin()));
  Location end_(createLocation(context->srcmgr, range_.getEnd()));

  /* -- open the user area */
  context->generator->startUserArea(begin_);

  /* -- visit the block (manipulate all assertions) */
  AssertVisitor visitor_(
      context, createLocation(context->srcmgr, range_.getBegin()));
  visitor_.TraverseStmt(stmt_);
  if(*context->failure)
    return false;

  /* -- finish rest of the block after last assertion */
  context->generator->copySource(
      visitor_.getCurrentLocation(),
      createLocation(context->srcmgr, range_.getEnd()));

  /* -- close the user area */
  context->generator->endUserArea(end_);

  return true;
}

bool SuiteVisitor::parseCaseBody(
    clang::NamespaceDecl* ns_) {

  for(
      auto iter_(ns_->decls_begin());
      iter_ != ns_->decls_end();
      ++iter_) {
    switch(context->state) {
      case ParserContext::CASE_BEGIN: {
        /* -- case variable */
        if(clang::isa<clang::VarDecl>(*iter_)) {
          auto vardecl_(clang::cast<clang::VarDecl>(*iter_));
          if(!parseVariable(vardecl_))
            return false;
          continue;
        }

        /* -- start up or tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          auto body_(getFunctionBody(fce_));
          if(body_ == nullptr)
            return false;

          /* -- suite start up */
          if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
            context->state = ParserContext::CASE_TEAR_DOWN;
            context->generator->caseStartUp();
            if(!parseCodeBlock(body_))
              return false;
            continue;
          }

          /* -- suite tear up */
          if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
            context->state = ParserContext::CASE_STATES;
            context->generator->caseStartUp();
            context->generator->emptyBody();
            context->generator->caseTearDown();
            if(!parseCodeBlock(body_))
              return false;
            continue;
          }

          /* -- state function */
          if(hasAnnotation(fce_, STATE_ANNOTATION)) {
            context->state = ParserContext::CASE_STATES;
            context->generator->caseStartUp();
            context->generator->emptyBody();
            context->generator->caseTearDown();
            context->generator->emptyBody();
            context->generator->enterState(fce_->getNameAsString());
            if(!parseCodeBlock(body_))
              return false;
            context->generator->leaveState();
            continue;
          }

          context->setError("case functions are not supported yet!", fce_);
          return false;
        }

        context->setError("Invalid case item", *iter_);
        return false;
      }

      case ParserContext::CASE_TEAR_DOWN: {
        /* -- tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          auto body_(getFunctionBody(fce_));
          if(body_ == nullptr)
            return false;

          /* -- suite start up */
          if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
            context->setError("the start-up function is already defined", fce_);
            return false;
          }

          /* -- suite tear up */
          if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
            context->state = ParserContext::CASE_STATES;
            context->generator->caseTearDown();
            if(!parseCodeBlock(body_))
              return false;
            continue;
          }

          /* -- state function */
          if(hasAnnotation(fce_, STATE_ANNOTATION)) {
            context->state = ParserContext::CASE_STATES;
            context->generator->caseTearDown();
            context->generator->emptyBody();
            context->generator->enterState(fce_->getNameAsString());
            if(!parseCodeBlock(body_))
              return false;
            context->generator->leaveState();
            continue;
          }

          context->setError("case functions are not supported yet!", fce_);
          return false;
        }

        context->setError("Invalid case item", *iter_);
        return false;
      }

      case ParserContext::CASE_STATES: {
        /* -- tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          auto body_(getFunctionBody(fce_));
          if(body_ == nullptr)
            return false;

          /* -- suite start up */
          if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
            context->setError("the start-up function is already defined", fce_);
            return false;
          }

          /* -- suite tear up */
          if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
            context->setError("the tear-down function is already defined", fce_);
            return false;
          }

          /* -- state function */
          if(hasAnnotation(fce_, STATE_ANNOTATION)) {
            context->generator->enterState(fce_->getNameAsString());
            if(!parseCodeBlock(body_))
              return false;
            context->generator->leaveState();
            continue;
          }

          context->setError("case functions are not supported yet!", fce_);
          return false;
        }

        context->setError("Invalid case item", *iter_);
        return false;
      }

      default:
        context->setError("Invalid case item", *iter_);
        return false;
    }
  }

  /* -- handle empty case */
  switch(context->state) {
    case ParserContext::CASE_BEGIN:
      context->generator->caseStartUp();
      context->generator->emptyBody();
      /* -- missing break is expected */
    case ParserContext::CASE_TEAR_DOWN:
      context->generator->caseTearDown();
      context->generator->emptyBody();
      break;
    case ParserContext::CASE_STATES:
      break;
    default:
      context->setError("invalid format of the case", ns_);
      return false;
  }

  return true;
}

bool SuiteVisitor::parseSuiteBody(
    clang::NamespaceDecl* ns_) {

    for(
        auto iter_(ns_->decls_begin());
        iter_ != ns_->decls_end();
        ++iter_) {
      switch(context->state) {
        case ParserContext::SUITE_BEGIN: {
          /* -- suite variable */
          if(clang::isa<clang::VarDecl>(*iter_)) {
            auto vardecl_(clang::cast<clang::VarDecl>(*iter_));
            if(!parseVariable(vardecl_))
              return false;
            continue;
          }

          /* -- start up or tear down functions */
          if(clang::isa<clang::FunctionDecl>(*iter_)) {
            auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
            auto body_(getFunctionBody(fce_));
            if(body_ == nullptr)
              return false;

            /* -- suite start up */
            if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
              context->state = ParserContext::SUITE_TEAR_DOWN;
              context->generator->suiteStartUp();
              if(!parseCodeBlock(body_))
                return false;
              continue;
            }

            /* -- suite tear up */
            if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
              context->state = ParserContext::SUITE_CASES;
              context->generator->suiteStartUp();
              context->generator->emptyBody();
              context->generator->suiteTearDown();
              if(!parseCodeBlock(body_))
                return false;
              continue;
            }

            context->setError("suite functions are not supported yet!", fce_);
            return false;
          }

          /* -- test case */
          if(clang::isa<clang::NamespaceDecl>(*iter_)) {
            auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
            if(hasAnnotation(casens_, CASE_ANNOTATION)) {
              context->state = ParserContext::CASE_BEGIN;
              context->generator->suiteStartUp();
              context->generator->emptyBody();
              context->generator->suiteTearDown();
              context->generator->emptyBody();
              context->generator->enterCase(casens_->getNameAsString());
              if(!parseCaseBody(casens_))
                return false;
              context->generator->leaveCase();
              context->state = ParserContext::SUITE_CASES;
              continue;
            }
          }

          context->setError("Invalid suite item", *iter_);
          return false;
        }

        case ParserContext::SUITE_TEAR_DOWN: {
          /* -- tear down functions */
          if(clang::isa<clang::FunctionDecl>(*iter_)) {
            auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
            auto body_(getFunctionBody(fce_));
            if(body_ == nullptr)
              return false;

            /* -- suite start up */
            if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
              context->setError("the start-up function is already defined", fce_);
              return false;
            }

            /* -- suite tear up */
            if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
              context->state = ParserContext::SUITE_CASES;
              context->generator->suiteTearDown();
              if(!parseCodeBlock(body_))
                return false;
              continue;
            }

            context->setError("suite functions are not supported yet!", fce_);
            return false;
          }

          /* -- test case */
          if(clang::isa<clang::NamespaceDecl>(*iter_)) {
            auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
            if(hasAnnotation(casens_, CASE_ANNOTATION)) {
              context->state = ParserContext::CASE_BEGIN;
              context->generator->suiteTearDown();
              context->generator->emptyBody();
              context->generator->enterCase(casens_->getNameAsString());
              if(!parseCaseBody(casens_))
                return false;
              context->generator->leaveCase();
              context->state = ParserContext::SUITE_CASES;
              continue;
            }
            /* -- test case */
          }

          context->setError("Invalid suite item", *iter_);
          return false;
        }

        case ParserContext::SUITE_CASES: {
          /* -- tear down functions */
          if(clang::isa<clang::FunctionDecl>(*iter_)) {
            auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
            auto body_(getFunctionBody(fce_));
            if(body_ == nullptr)
              return false;

            /* -- suite start up */
            if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
              context->setError("the start-up function is already defined", fce_);
              return false;
            }

            /* -- suite tear up */
            if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
              context->setError("the tear-down function is already defined", fce_);
              return false;
            }

            context->setError("suite functions are not supported yet!", fce_);
            return false;
          }

          /* -- test case */
          if(clang::isa<clang::NamespaceDecl>(*iter_)) {
            auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
            if(hasAnnotation(casens_, CASE_ANNOTATION)) {
              context->state = ParserContext::CASE_BEGIN;
              context->generator->enterCase(casens_->getNameAsString());
              if(!parseCaseBody(casens_))
                return false;
              context->generator->leaveCase();
              context->state = ParserContext::SUITE_CASES;
              continue;
            }
            /* -- test case */
          }

          context->setError("Invalid suite item", *iter_);
          return false;
        }

        default:
          context->setError("Invalid suite item", *iter_);
          return false;
      }
    }

    /* -- handle empty suite */
    switch(context->state) {
      case ParserContext::SUITE_BEGIN:
        context->generator->suiteStartUp();
        context->generator->emptyBody();
        /* -- missing break is expected */
      case ParserContext::SUITE_TEAR_DOWN:
        context->generator->suiteTearDown();
        context->generator->emptyBody();
        break;
      case ParserContext::SUITE_CASES:
        break;
      default:
        context->setError("invalid format of the suite", ns_);
        return false;
    }

  return true;
}

bool SuiteVisitor::parseSuite(
    clang::NamespaceDecl* ns_) {
  /* -- copy the input file */
  context->copyInput(ns_, false);

  /* -- enter the suite */
  std::string suitename_(ns_->getNameAsString());
  context->generator->enterSuite(suitename_);

  /* -- parse body of the suite */
  context->state = ParserContext::SUITE_BEGIN;
  if(!parseSuiteBody(ns_))
    return false;
  context->state = ParserContext::ROOT;

  /* -- leave the suite */
  context->generator->leaveSuite();

  /* -- skip the body of the function */
  context->moveToEnd(ns_);

  return true;
}

bool SuiteVisitor::VisitNamespaceDecl(
    clang::NamespaceDecl* ns_) {
  if(hasAnnotation(ns_, SUITE_ANNOTATION)) {
    if(!parseSuite(ns_))
      return false;
  }

  return true;
}

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

/* -- command line options */
llvm::cl::OptionCategory ParserOptCategory("otest2 options");
llvm::cl::extrahelp CommonHelp(
    clang::tooling::CommonOptionsParser::HelpMessage);
llvm::cl::extrahelp MoreHelp("\nOTest2 preprocessor");

void parse(
    const Options& options_) {
  /* -- prepare the options */
  std::vector<std::string> opts_;
  opts_.push_back("otest2");
  opts_.push_back("--extra-arg-before=-xc++");
  opts_.push_back("--extra-arg-before=--driver-mode=g++");
  opts_.push_back("--extra-arg-before=--std=c++11");
  opts_.push_back("--extra-arg-before=-DOTEST2_PARSER_ACTIVE");
  for(const auto& path_ : options_.includes) {
    opts_.push_back("--extra-arg-before=-I" + path_);
  }
  opts_.push_back(options_.infile);

  std::vector<const char*> argv_;
  for(const auto& arg_ : opts_)
    argv_.push_back(arg_.c_str());

  /* -- prepare the reader and the output generator */
  std::ifstream ifs_(options_.infile);
  FileReader reader_(&ifs_);
  std::ostream *os_(nullptr);
  std::ofstream ofs_;
  if(options_.outfile == "-") {
    os_ = &std::cout;
  }
  else {
    ofs_.open(options_.outfile);
    os_ = &ofs_;
  }
  GeneratorStd generator_(
      os_,
      &reader_,
      options_.domain,
      options_.infile,
      options_.outfile);
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
      tool_.run(new FrontendFactory(&generator_, &failure_, &exception_)));
  if(retval_) {
    /* -- remove the half created file */
    unlink(options_.outfile.c_str());

    throw ParserFailure();
  }

  /* -- report an error */
  if(failure_)
    throw ParserException(std::move(exception_));
}

} /* -- namespace OTest2 */

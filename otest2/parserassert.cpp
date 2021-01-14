/*
 * Copyright (C) 2020 Ondrej Starek
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
#include "parserassert.h"

#include <clang/AST/Decl.h>
#include <string>

#include "generator.h"
#include "parserannotationimpl.h"
#include "parsercode.h"
#include "parsercontextimpl.h"
#include "parsertype.h"
#include "typetemplate.h"

#include <iostream>

namespace OTest2 {

namespace Parser {

namespace {

std::vector<std::string> parseAnnotationArguments(
    const AnnotationRegex& cmp_) {
  auto annot_args_(cmp_.matches[1].str());
  std::regex split_("(;)");
  std::sregex_token_iterator iter_(
      annot_args_.begin(), annot_args_.end(), split_, -1);
  return std::vector<std::string>(iter_, std::sregex_token_iterator());
}

} /* -- namespace */

AssertVisitor::AssertVisitor(
    ParserContext* context_,
    const Location& curr_) :
  context(context_),
  current(curr_),
  subtree_root(nullptr) {

}

AssertVisitor::~AssertVisitor() {

}

bool AssertVisitor::generateAssertion(
    clang::CallExpr* expr_,
    clang::Decl* declref_,
    clang::FunctionDecl* fce_,
    std::string class_arg_,
    std::string method_arg_,
    std::vector<std::string> template_params_,
    std::vector<Generator::AssertionArg> args_ranges_,
    int ignored_params_) {
  /* -- if the function is a template function, expand the list of template
   *    parameters by the deduced types. */
  if(fce_->isTemplateInstantiation()) {
    const clang::TemplateArgumentList* args_(fce_->getTemplateSpecializationArgs());
    if(args_ != nullptr) {
      for(int i_(0); i_ < static_cast<int>(args_->size()); ++i_) {
        const auto& arg_(args_->get(i_));
        if(arg_.getKind() == clang::TemplateArgument::Type) {
          auto type_(arg_.getAsType());
          template_params_.push_back(type_.getAsString());
        } else if(arg_.getKind() == clang::TemplateArgument::Template) {
          auto templ_(arg_.getAsTemplate().getAsTemplateDecl());
          template_params_.push_back(parseType(context, *templ_));
        }
      }
    }
  }

  /* -- expand the assertion templates */
  if(!expandTemplate(class_arg_, template_params_)) {
    context->setError("invalid type template of the assertion class", declref_);
    return false;
  }
  if(!expandTemplate(method_arg_, template_params_)) {
    context->setError("invalid type template of the assertion method", declref_);
    return false;
  }

  /* -- copy source just before the assertion */
  clang::SourceRange expr_range_(context->getNodeRange(expr_));
  auto expr_begin_(context->createLocation(expr_range_.getBegin()));
  context->generator->copySource(current, expr_begin_);

  /* -- find first and last non-default argument */
  const int argnum_(expr_->getNumArgs());
  int last_index_(-1);
  for(int i_(0); i_ < argnum_; ++i_) {
    clang::Expr *arg_(expr_->getArg(i_));
    if(clang::isa<clang::CXXDefaultArgExpr>(arg_))
      break;
    last_index_ = i_;
  }
  if(last_index_ < 0 || last_index_ < (ignored_params_ - 1)) {
    context->setError("there are not enough arguments of the assertion", fce_);
    return false;
  }

  /* -- Get range of the source code containing assertion's parameters.
   *    A couple of first parameters may be ignored (they are parsed by
   *    the expression assertion functions. */
  auto endarg_(expr_->getArg(last_index_));
  clang::SourceRange endrange_(context->getNodeRange(endarg_));
  auto args_end_(context->createLocation(endrange_.getEnd()));
  if(last_index_ >= ignored_params_) {
    /* -- if there are some not ignored parameters add them into the list
     *    of parameter ranges in the source file. */
    auto begarg_(expr_->getArg(ignored_params_));
    clang::SourceRange begrange_(context->getNodeRange(begarg_));
    /* -- first parameter is used as a message in the generic assertions */
    args_ranges_.push_back({
        context->createLocation(begrange_.getBegin()),
        context->createLocation(begrange_.getEnd()),
        false});
    args_ranges_.push_back({
        context->createLocation(begrange_.getEnd()),
        args_end_,
        false});
  }
  context->generator->makeAssertion(class_arg_, method_arg_, args_ranges_);

  /* -- keep last position for next source copying */
  current = args_end_;

  return true;

}

bool AssertVisitor::visitAssertion(
    clang::CallExpr* expr_,
    clang::Decl* declref_,
    clang::FunctionDecl* fce_,
    const AnnotationRegex& cmp_) {
  /* -- split the annotation arguments */
  std::vector<std::string> annotation_args_(parseAnnotationArguments(cmp_));
  if(annotation_args_.size() != 2) {
    context->setError("invalid count of the arguments of the assertion annotation", declref_);
    return false;
  }

  /* -- generate the assertion */
  return generateAssertion(
      expr_,
      declref_,
      fce_,
      annotation_args_[0],
      annotation_args_[1],
      std::vector<std::string>(),
      std::vector<Generator::AssertionArg>(),
      0);
}

bool AssertVisitor::visitAssertionExpr(
    clang::CallExpr* expr_,
    clang::Decl* declref_,
    clang::FunctionDecl* fce_,
    const AnnotationRegex& cmp_) {
  /* -- split the annotation arguments:
   *       0: assertion class
   *       1: generic asssertion function
   *       2: comparison assertion function
   */
  std::vector<std::string> annotation_args_(parseAnnotationArguments(cmp_));
  if(annotation_args_.size() != 3) {
    context->setError("invalid count of the arguments of the assertion annotation", declref_);
    return false;
  }

  /* -- Parse the expression passed as the first argument. Firstly, ignore
   *    parenthesis wrapping the expression. */
  auto firstarg_(expr_->getArg(0));
  while(clang::isa<clang::ParenExpr>(firstarg_)) {
    auto par_expr_(clang::cast<clang::ParenExpr>(firstarg_));
    firstarg_ = par_expr_->getSubExpr();
  }

  /* -- If the root operator is a relation operator redirect the assertion
   *    to the comparison method with a selected comparator type. Otherwise,
   *    use the generic assertion method. */
  std::vector<std::string> template_args_;
  std::vector<Generator::AssertionArg> args_ranges_;
  int ignored_args_(0);
  int method_index_(1);
  if(clang::isa<clang::BinaryOperator>(firstarg_)) {
    auto oper_(clang::cast<clang::BinaryOperator>(firstarg_));
    std::string comparator_;
    switch(oper_->getOpcode()) {
      case clang::BO_EQ:
        comparator_ = "Equal";
        break;
      case clang::BO_NE:
        comparator_ = "NotEqual";
        break;
      case clang::BO_LT:
        comparator_ = "Less";
        break;
      case clang::BO_LE:
        comparator_ = "LessOrEqual";
        break;
      case clang::BO_GT:
        comparator_ = "Greater";
        break;
      case clang::BO_GE:
        comparator_ = "GreaterOrEqual";
        break;
      default:
        break;
    }

    if(!comparator_.empty()) {
      /* -- source ranges of the operands */
      auto left_(oper_->getLHS());
      clang::SourceRange leftrange_(context->getNodeRange(left_));
      args_ranges_.push_back({
          context->createLocation(leftrange_.getBegin()),
          context->createLocation(leftrange_.getEnd()),
          true});
      auto right_(oper_->getRHS());
      clang::SourceRange rightrange_(context->getNodeRange(right_));
      args_ranges_.push_back({
          context->createLocation(rightrange_.getBegin()),
          context->createLocation(rightrange_.getEnd()),
          true});

      /* -- add the comparator into the template parameters */
      template_args_.push_back("OTest2::" + comparator_);

      /* -- ignore the first argument - I have already parsed it */
      ignored_args_ = 1;

      /* -- select the comparison method */
      method_index_ = 2;
    }
  }

  /* -- generate the assertion */
  return generateAssertion(
      expr_,
      declref_,
      fce_,
      annotation_args_[0],
      annotation_args_[method_index_],
      template_args_,
      args_ranges_,
      ignored_args_);
}

bool AssertVisitor::visitSwitchState(
    clang::CallExpr* expr_,
    clang::Decl* declref_,
    clang::FunctionDecl* fce_) {
  /* -- find first and last non-default argument */
  const int argnum_(expr_->getNumArgs());
  if(argnum_ != 2) {
    context->setError("invalid count of the state switching function", fce_);
    return false;
  }

  /* -- copy source just before the state switch */
  clang::SourceRange expr_range_(context->getNodeRange(expr_));
  auto expr_begin_(context->createLocation(expr_range_.getBegin()));
  auto expr_end_(context->createLocation(expr_range_.getEnd()));
  context->generator->copySource(current, expr_begin_);

  /* -- get the arguments */
  auto state_(expr_->getArg(0));
  clang::SourceRange state_range_(context->getNodeRange(state_));
  auto state_begin_(context->createLocation(state_range_.getBegin()));
  auto state_end_(context->createLocation(state_range_.getEnd()));
  auto delay_(expr_->getArg(1));
  clang::SourceRange delay_range_(context->getNodeRange(delay_));
  auto delay_begin_(context->createLocation(delay_range_.getBegin()));
  auto delay_end_(context->createLocation(delay_range_.getEnd()));

  /* -- generate the state switch */
  context->generator->makeStateSwitch(
      state_begin_,
      state_end_,
      delay_begin_,
      delay_end_);

  /* -- keep last position for next source copying */
  current = expr_end_;

  return true;
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

  /* -- parse name of the assertion */
  auto fce_(clang::cast<clang::FunctionDecl>(declref_));

  /* -- check the assertion annotation */
  AnnotationRegex cmp_(ASSERTION_ANNOTATION);
  if(hasAnnotation(declref_, cmp_))
    return visitAssertion(expr_, declref_, fce_, cmp_);

  AnnotationRegex cmp_expr_(ASSERTION_EXPR_ANNOTATION);
  if(hasAnnotation(declref_, cmp_expr_))
    return visitAssertionExpr(expr_, declref_, fce_, cmp_expr_);

  /* -- check the switchState annotation */
  if(hasAnnotation(declref_, SWITCH_STATE_ANNOTATION))
    return visitSwitchState(expr_, declref_, fce_);

  /* -- no special function - let continue */
  return true;
}

bool AssertVisitor::VisitCXXTryStmt(
    clang::CXXTryStmt* stmt_) {
  /* -- sanity check, it shouldn't happen */
  const int handlers_(stmt_->getNumHandlers());
  if(handlers_ < 1)
    return true;

  /* -- check annotations of handlers */
  bool all_clear_(true);
  bool all_marked_(true);
  for(int i_(0); i_ < handlers_; ++i_) {
    const clang::CXXCatchStmt* handler_(stmt_->getHandler(i_));
    auto exc_decl_(handler_->getExceptionDecl());
    if(hasAnnotation(exc_decl_, CATCH_ANNOTATION)) {
      all_clear_ = false;
    }
    else {
      all_marked_ = false;
    }
  }
  /* -- all handlers are clear => ordinary try/catch statement */
  if(all_clear_)
    return true;
  /* -- some handlers are marked, some not => invalid use of the try/catch */
  if(!all_marked_) {
    context->setError("mixed marked and unmarked catch statements!", stmt_);
    return false;
  }

  /* -- copy previous part of the source file */
  clang::SourceRange range_(context->getNodeRange(stmt_));
  auto begin_(context->createLocation(range_.getBegin()));
  auto end_(context->createLocation(range_.getEnd()));
  context->generator->copySource(current, begin_);

  /* -- begin the asserted try/catch code */
  context->generator->endUserArea(begin_);
  context->generator->makeTryCatchBegin(begin_);

  /* -- generate the asserted try/catch statement */
  if(!parseCodeBlock(context, stmt_->getTryBlock()))
    return false;

  for(int i_(0); i_ < handlers_; ++i_) {
    const clang::CXXCatchStmt* handler_(stmt_->getHandler(i_));

    /* -- generate catch declaration */
    auto vardecl_(handler_->getExceptionDecl());
    context->generator->makeCatchHandler(
        parseType(context, vardecl_->getType()),
        vardecl_->getNameAsString());

    /* -- process content of the handler block */
    auto handler_block_(handler_->getHandlerBlock());
    if(!parseCodeBlock(context, handler_block_))
      return false;
  }
  context->generator->makeTryCatchEnd();

  /* -- skip rest of the original try/catch block */
  context->generator->startUserArea(end_);
  current = end_;
  subtree_root = stmt_;

  return true;
}

bool AssertVisitor::dataTraverseStmtPre(
    clang::Stmt* stmt_) {
  return subtree_root == nullptr;
}

bool AssertVisitor::dataTraverseStmtPost(
    clang::Stmt* stmt_) {
  if(subtree_root == stmt_)
    subtree_root = nullptr;
  return true;
}

Location AssertVisitor::getCurrentLocation() const {
  return current;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

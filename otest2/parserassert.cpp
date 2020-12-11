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

AssertVisitor::AssertVisitor(
    ParserContext* context_,
    const Location& curr_) :
  context(context_),
  current(curr_),
  subtree_root(nullptr) {

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

  /* -- check the assertion annotation */
  AnnotationRegex cmp_(ASSERTION_ANNOTATION);
  if(hasAnnotation(declref_, cmp_)) {
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
          } else if(arg_.getKind() == clang::TemplateArgument::Template) {
            auto templ_(arg_.getAsTemplate().getAsTemplateDecl());
            template_params_.push_back(parseType(context, *templ_));
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
    clang::SourceRange expr_range_(context->getNodeRange(expr_));
    auto expr_begin_(context->createLocation(expr_range_.getBegin()));
    context->generator->copySource(current, expr_begin_);

    /* -- Generate the assertion. I need to know the range of arguments
     *    because I want to insert the first argument as a text (to be shown
     *    in the assertion message. */
    auto begarg_(expr_->getArg(0));
    auto endarg_(expr_->getArg(last_index_));
    clang::SourceRange begrange_(context->getNodeRange(begarg_));
    clang::SourceRange endrange_(context->getNodeRange(endarg_));
    auto args_begin_(context->createLocation(begrange_.getBegin()));
    auto args_end_(context->createLocation(endrange_.getEnd()));
    auto msg_end_(context->createLocation(begrange_.getEnd()));
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

  /* -- check the switchState annotation */
  if(hasAnnotation(declref_, SWITCH_STATE_ANNOTATION)) {
    /* -- parse name of the assertion */
    auto fce_(clang::cast<clang::FunctionDecl>(declref_));
    auto fcename_(fce_->getName().str());

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

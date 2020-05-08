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
#include "parserfunction.h"

#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceLocation.h>
#include <memory>
#include <string>

#include "function.h"
#include "generator.h"
#include "location.h"
#include "parserannotationimpl.h"
#include "parsercode.h"
#include "parsercontextimpl.h"
#include "parsertype.h"

#include <iostream>

namespace OTest2 {

namespace Parser {

FunctionFlags::FunctionFlags(
    bool test_state_) :
  start_up(true),
  tear_down(true),
  test_state(test_state_),
  first_state(true) {

}

FunctionPtr createFunctionObject(
    ParserContext* context_,
    FunctionAccess access_,
    const std::string& instance_,
    const std::string& classname_,
    clang::FunctionDecl* fce_) {
  /* -- create the object */
  std::string fce_name_(fce_->getNameAsString());
  std::string ret_type_(fce_->getReturnType().getAsString());
  auto function_(std::make_shared<Function>(
      access_, instance_, classname_, fce_name_, ret_type_));

  /* -- parse function parameters */
  const int parnum_(fce_->getNumParams());
  for(int i_(0); i_ < parnum_; ++i_) {
    auto paramdecl_(fce_->getParamDecl(i_));

    auto param_name_(paramdecl_->getNameAsString());
    auto param_type_(parseType(context_, paramdecl_->getType()));

    if(param_type_ == "const OTest2::Context &") {
      /* -- passed OTest2 context */
      function_->addContextParameter(param_name_);
    }
    else {
      /* -- user data passed as function parameters */

      /* -- check the user data annotation */
      auto param_key_(param_name_);
      AnnotationRegex annotation_(USER_DATA_VAR_ANNOTATION);
      bool user_data_(hasAnnotation(paramdecl_, annotation_));
      if(user_data_ && !annotation_.matches[1].str().empty())
        param_key_ = annotation_.matches[1].str();

      /* -- unknown key for user data */
      if(param_key_.empty()) {
        context_->setError("neither name nor key is specified for user data", paramdecl_);
        return nullptr;
      }

      /* -- register the parameter */
      function_->addUserDataParameter(param_name_, param_key_, param_type_);
    }
  }

  return function_;
}

bool isAllowedFunctionDeclaration(
    ParserContext* context_,
    clang::FunctionDecl* fce_) {
  AnnotationAny any_annotation_;
  if(!fce_->doesThisDeclarationHaveABody()) {
    /* -- user functions may be declared forwardly */
    if(!hasAnnotation(fce_, any_annotation_))
      return true;
  }
  return false;
}

std::pair<bool, bool> parseFunction(
    ParserContext* context_,
    clang::FunctionDecl* fce_,
    FunctionFlags& fce_flags_) {
  AnnotationAny any_annotation_;
  if(!fce_->doesThisDeclarationHaveABody()) {
    /* -- user functions may be declared forwardly */
    if(!hasAnnotation(fce_, any_annotation_)) {
      return {true, false};
    }

    /* -- the test states may be declared forwardly too */
    if(hasAnnotation(fce_, STATE_ANNOTATION)) {
      if(!fce_flags_.test_state) {
        context_->setError("the state function is not allowed here", fce_);
        return {false, true};
      }
      return {true, true};
    }

    /* -- otherwise function without body is an error */
    context_->setError("function must have a body", fce_);
    return {false, false};
  }

  /* -- get function body */
  auto body_(fce_->getBody());

  /* -- source range of the function declaration */
  clang::SourceRange fce_range_(context_->getNodeRange(fce_));
  Location decl_begin_(context_->createLocation(fce_range_.getBegin()));
  clang::SourceRange body_range_(context_->getNodeRange(body_));
  Location decl_end_(context_->createLocation(body_range_.getBegin()));

  /* -- create description of the function */
  FunctionPtr function_(
      createFunctionObject(context_, FunctionAccess::NONE, "", "", fce_));
  if(function_ == nullptr)
    return {false, false};

  /* -- generate function declaration */
  if(hasAnnotation(fce_, any_annotation_)) {

    if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
      if(!fce_flags_.start_up || !fce_flags_.first_state) {
        context_->setError("unexpected start-up function", fce_);
        return {false, false};
      }
      fce_flags_.start_up = false; /* -- avoid second function */

      context_->generator->appendStartUpFunction(
          function_, decl_begin_, decl_end_);
      if(!parseCodeBlock(context_, body_))
        return {false, false};

      return {true, false};
    }
    else if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
      if(!fce_flags_.tear_down || !fce_flags_.first_state) {
        context_->setError("unexpected tear-down function", fce_);
        return {false, false};
      }
      fce_flags_.tear_down = false; /* -- avoid second function */

      context_->generator->appendTearDownFunction(
          function_, decl_begin_, decl_end_);
      if(!parseCodeBlock(context_, body_))
        return {false, false};

      return {true, false};
    }
    else if(hasAnnotation(fce_, STATE_ANNOTATION)) {
      if(!fce_flags_.test_state) {
        context_->setError("unexpected test state function", fce_);
        return {false, true};
      }

      /* -- finish block of case's functions (first state function) */
      if(fce_flags_.first_state) {
        context_->generator->finishCaseFunctions();
        fce_flags_.first_state = false;
      }

      /* -- enter the test case */
      context_->generator->enterState(
          fce_->getNameAsString(), function_, decl_begin_, decl_end_);
      if(!parseCodeBlock(context_, body_))
        return {false, true};
      context_->generator->leaveState();

      return {true, true};
    }
    else {
      context_->setError("invalid function annotation", fce_);
      return {false, false};
    }
  }
  else {
    if(!fce_flags_.first_state) {
      context_->setError("unexpected user function", fce_);
      return {false, false};
    }

    context_->generator->appendGenericFunction(
        function_, decl_begin_, decl_end_);
    if(!parseCodeBlock(context_, body_))
      return {false, false};

    return {true, false};
  }
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

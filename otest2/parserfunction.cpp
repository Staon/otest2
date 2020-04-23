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
#include "parserannotation.h"
#include "parsercode.h"
#include "parsercontextimpl.h"

#include <iostream>

namespace OTest2 {

namespace Parser {

namespace {

FunctionPtr createFunctionObject(
    ParserContext* context_,
    clang::FunctionDecl* fce_) {
  /* -- create the object */
  std::string fce_name_(fce_->getNameAsString());
  auto function_(std::make_shared<Function>(fce_name_));

  /* -- parse function parameters */
  const int parnum_(fce_->getNumParams());
  for(int i_(0); i_ < parnum_; ++i_) {
    auto paramdecl_(fce_->getParamDecl(i_));

    auto param_name_(paramdecl_->getNameAsString());
    auto param_key_(param_name_);
    auto param_type_(paramdecl_->getType().getAsString());

    /* -- check the user data annotation */
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

  return function_;
}

} /* -- namespace */

bool parseFunction(
    ParserContext* context_,
    clang::FunctionDecl* fce_) {
  /* -- get function body */
  clang::Stmt* body_(nullptr);
  if(fce_->doesThisDeclarationHaveABody())
    body_ = fce_->getBody();

  /* -- source range of the function declaration */
  clang::SourceRange fce_range_(context_->getNodeRange(fce_));
  Location decl_begin_(context_->createLocation(fce_range_.getBegin()));
  Location decl_end_;
  if(body_ != nullptr) {
    clang::SourceRange body_range_(context_->getNodeRange(body_));
    decl_end_ = context_->createLocation(body_range_.getBegin());
  }
  else {
    decl_end_ = context_->createLocation(fce_range_.getEnd());
  }

  /* -- generate function declaration */
  if(body_ != nullptr
      && (hasAnnotation(fce_, START_UP_ANNOTATION) || (hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)))) {
    FunctionPtr function_(createFunctionObject(context_, fce_));
    if(function_ == nullptr)
      return false;

    if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
      context_->generator->appendStartUpFunction(
          function_, decl_begin_, decl_end_);
    }
    else {
      context_->generator->appendTearDownFunction(
          function_, decl_begin_, decl_end_);
    }
  }
  else {
    context_->generator->appendGenericFunction(
        decl_begin_, decl_end_, body_ != nullptr);
  }

  /* -- process function body */
  if(body_ != nullptr) {
    if(!parseCodeBlock(context_, body_))
      return false;
  }

  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

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
#include "parsertype.h"

#include <assert.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Type.h>

#include "parsercontext.h"

namespace OTest2 {

namespace Parser {

namespace {

bool traverseMethodsOfClass(
    const clang::CXXRecordDecl* object_,
    std::function<bool(const clang::CXXMethodDecl*)> fce_) {
  for(const auto* method_ : object_->methods()) {
    if(fce_(method_))
      return true;
  }
  return false;
}

} /* -- namespace */

std::string parseType(
    ParserContext* context_,
    const clang::QualType& type_) {
  clang::PrintingPolicy policy_(*context_->langopts);
  policy_.adjustForCPlusPlus();
  policy_.SuppressUnwrittenScope = true;
  policy_.FullyQualifiedName = true;
  return type_.getAsString(policy_);
}

bool traverseMethods(
    ParserContext* context_,
    const clang::CXXRecordDecl* object_,
    bool post_order_,
    std::function<bool(const clang::CXXMethodDecl*)> fce_) {
  assert(object_ != nullptr);

  /* -- try me, if the pre-order traversing is requested */
  if(!post_order_) {
    if(traverseMethodsOfClass(object_, fce_))
      return true;
  }

  /* -- try base classes */
  for(const auto& base_ : object_->bases()) {
    clang::CXXRecordDecl* base_class_(base_.getType().getTypePtr()->getAsCXXRecordDecl());
    if(traverseMethods(context_, base_class_, post_order_, fce_))
      return true;
  }

  /* -- try me, if the post-order traversing is requested */
  if(post_order_) {
    if(traverseMethodsOfClass(object_, fce_))
      return true;
  }

  return false;
}

bool traverseClasses(
    ParserContext* context_,
    const clang::CXXRecordDecl* object_,
    bool post_order_,
    std::function<bool(const clang::CXXRecordDecl*)> fce_) {
  assert(object_ != nullptr);

  /* -- try me, if the pre-order traversing is requested */
  if(!post_order_) {
    if(fce_(object_))
      return true;
  }

  /* -- try base classes */
  for(const auto& base_ : object_->bases()) {
    clang::CXXRecordDecl* base_class_(base_.getType().getTypePtr()->getAsCXXRecordDecl());
    if(traverseClasses(context_, base_class_, post_order_, fce_))
      return true;
  }

  /* -- try me, if the post-order traversing is requested */
  if(post_order_) {
    if(fce_(object_))
      return true;
  }

  return false;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

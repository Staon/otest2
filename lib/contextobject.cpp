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

#include <contextobject.h>

#include <iostream>
#include <sstream>

#include <assertbean.h>
#include <context.h>
#include <reporter.h>
#include <semanticstack.h>

namespace OTest2 {

ContextObject::ContextObject() {

}

ContextObject::~ContextObject() {

}

void ContextObject::otest2AssertGeneric(
    const std::string& file_,
    int lineno_,
    const std::string& expression_,
    const std::string& message_,
    bool condition_) {
  const Context& context_(otest2Context());

  /* -- change the result of current object */
  if(!condition_)
    context_.semantic_stack->setTop(false);

  /* -- report the assertion */
  std::ostringstream sos_;
  if(!expression_.empty())
    sos_ << expression_;
  if(!message_.empty()) {
    if(!expression_.empty())
      sos_ << " ";
    sos_ << message_;
  }
  context_.reporter->reportAssert(
      context_, condition_, sos_.str(), file_, lineno_);
}

void ContextObject::testAssert(
    const std::string& file_,
    int lineno_,
    const std::string& expression_,
    bool condition_) {
  otest2AssertGeneric(file_, lineno_, expression_, "", condition_);
}

void ContextObject::testAssert(
    const std::string& file_,
    int lineno_,
    const std::string& expression_,
    const AssertBean& bean_) {
  otest2AssertGeneric(
      file_, lineno_, expression_, bean_.getMessage(), bean_.getCondition());
}

} /* namespace OTest2 */

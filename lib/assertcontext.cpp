/*
 * Copyright (C) 2019 Ondrej Starek
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

#include <otest2/assertcontext.h>

#include <sstream>

#include <otest2/context.h>
#include <otest2/reporter.h>
#include <otest2/semanticstack.h>

namespace OTest2 {

AssertContext::AssertContext(
    const Context& context_,
    const std::string& file_,
    int lineno_,
    const std::string expression_) :
  context(&context_),
  file(file_),
  lineno(lineno_),
  expression(expression_) {

}

AssertContext::~AssertContext() {

}

bool AssertContext::testAssertImpl(
    bool condition_,
    const std::string& message_,
    bool use_expression_) const {
  /* -- change the result of current object */
  if(!condition_)
    context->semantic_stack->setTop(false);

  /* -- report the assertion */
  std::ostringstream sos_;
  if(use_expression_ && !expression.empty())
    sos_ << expression;
  if(!message_.empty()) {
    if(use_expression_ && !expression.empty())
      sos_ << " ";
    sos_ << message_;
  }
  context->reporter->reportAssert(
      *context, condition_, sos_.str(), file, lineno);

  return condition_;
}

} /* namespace OTest2 */
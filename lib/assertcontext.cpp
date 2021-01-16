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

#include <assertcontext.h>

#include <assert.h>

#include <assertstream.h>
#include <context.h>
#include <reporter.h>
#include <semanticstack.h>

namespace OTest2 {

AssertContext::AssertContext(
    const Context& context_,
    const std::string& file_,
    int lineno_,
    std::initializer_list<const char*> parameters_) :
  context(&context_),
  file(file_),
  lineno(lineno_),
  parameters(parameters_) {

}

AssertContext::~AssertContext() = default;

AssertStream AssertContext::enterAssertion(
    bool result_) {
  /* -- change the result of current object */
  if(!result_)
    context->semantic_stack->setTop(false);

  /* -- open the assertion for additional messages */
  auto assert_buffer_(context->reporter->enterAssert(
      *context, result_, file, lineno));

  return AssertStream(*context, assert_buffer_, result_, parameters);
}

bool AssertContext::simpleAssertionImpl(
    bool condition_,
    const std::string& message_,
    bool print_expression_) {
  /* -- open the assertion */
  AssertStream report_(enterAssertion(condition_));

  /* -- report the assertion */
  if(print_expression_)
    report_ << '\'' << assertPar(0) << '\'';
  if(!message_.empty()) {
    if(print_expression_)
      report_ << ' ';
    report_ << message_;
  }
  report_ << commitMsg();

  return report_.getResult();
}

const Context& AssertContext::otest2Context() const {
  return *context;
}

} /* namespace OTest2 */

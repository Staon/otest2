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

#include <cmddestroysuite.h>

#include <assert.h>

#include <context.h>
#include <reporter.h>
#include <semanticstack.h>
#include <suiteordinary.h>

namespace OTest2 {

CmdDestroySuite::CmdDestroySuite(
    SuiteOrdinaryPtr suite_) :
  suite(suite_) {
  assert(!suite.isNull());

}

CmdDestroySuite::~CmdDestroySuite() {

}

void CmdDestroySuite::run(
    const Context& context_) {
  /* -- clean up the suite */
  suite->tearDownSuite(context_);

  /* -- report finishing of the suite */
  context_.reporter->leaveSuite(
      context_, suite->getName(), context_.semantic_stack->top());

  /* -- return the value */
  context_.semantic_stack->popAnd();
}

} /* namespace OTest2 */

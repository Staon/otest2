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

#include <cmddestroycase.h>

#include <assert.h>

#include <context.h>
#include <objectpath.h>
#include <reporter.h>
#include <caseordinary.h>
#include <semanticstack.h>

namespace OTest2 {

CmdDestroyCase::CmdDestroyCase(
    CaseOrdinaryPtr case_) :
  testcase(case_) {
  assert(!testcase.isNull());

}

CmdDestroyCase::~CmdDestroyCase() {

}

void CmdDestroyCase::run(
    const Context& context_) {
  /* -- clean up the case */
  testcase->tearDownCase(context_);

  /* -- report finishing of the suite */
  context_.reporter->leaveCase(
      context_, testcase->getName(), context_.semantic_stack->top());
  context_.object_path->popName();

  /* -- return the case value */
  context_.semantic_stack->popAnd();
}

} /* namespace OTest2 */

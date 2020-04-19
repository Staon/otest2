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

#include <cmdstartcase.h>

#include <assert.h>
#include <memory>

#include <cmddestroycase.h>
#include <cmdfirststate.h>
#include <commandstack.h>
#include <context.h>
#include <reporter.h>
#include <caseordinary.h>
#include <semanticstack.h>

namespace OTest2 {

CmdStartCase::CmdStartCase(
    CaseOrdinaryPtr case_) :
  testcase(case_) {
  assert(!testcase.isNull());

}

CmdStartCase::~CmdStartCase() {

}

void CmdStartCase::run(
    const Context& context_) {
  /* -- report start of the suite */
  context_.reporter->enterCase(context_, testcase->getName());

  /* -- prepare the return value */
  context_.semantic_stack->push(true);

  /* -- initialize the case */
  testcase->startUpCase(context_);
  if(!context_.semantic_stack->top()) {
    /* -- the initialization failed */
    context_.reporter->leaveCase(context_, testcase->getName(), false);
    context_.semantic_stack->popAnd();
  }
  else {
    /* -- clean-up of the case */
    context_.command_stack->pushCommand(
        std::make_shared<CmdDestroyCase>(testcase));

    /* -- run first test state */
    context_.command_stack->pushCommand(
        std::make_shared<CmdFirstState>(testcase));
  }
}

} /* namespace OTest2 */

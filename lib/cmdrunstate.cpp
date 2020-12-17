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

#include <cmdrunstate.h>

#include <assert.h>
#include <memory>

#include <cmddummy.h>
#include <commandstack.h>
#include <context.h>
#include <reporter.h>
#include <semanticstack.h>
#include <stateordinary.h>

namespace OTest2 {

CmdRunState::CmdRunState(
    CaseOrdinaryPtr parent_,
    StateOrdinaryPtr state_,
    bool wait_before_,
    int delay_) :
  parent(parent_),
  state(state_),
  wait_before(wait_before_),
  delay(delay_) {
  assert(parent != nullptr && state != nullptr && (!wait_before || delay >= 0));

}

CmdRunState::~CmdRunState() {

}

bool CmdRunState::shouldWait(
    const Context& context_,
    int& delay_) {
  if(wait_before) {
    delay_ = delay;
    return true;
  }
  else
    return false;
}

void CmdRunState::run(
    const Context& context_) {
  /* -- report the state entrance */
  context_.reporter->enterState(context_, state->getName());

  /* -- Prepare a dummy command. The test state can replace it by
   *    a command switching another state. */
  context_.command_stack->pushCommand(std::make_shared<CmdDummy>());
  /* -- prepare the return value of the state */
  context_.semantic_stack->push(true);
  /* -- execute the state */
  state->executeState(context_, parent);

  /* -- report end of the state */
  context_.reporter->leaveState(
      context_, state->getName(), context_.semantic_stack->top());

  /* -- return the value */
  context_.semantic_stack->popAnd();
}

} /* namespace OTest2 */

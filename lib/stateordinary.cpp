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

#include <stateordinary.h>

#include <assert.h>
#include <memory>

#include <cmdrunstate.h>
#include <commandstack.h>
#include <context.h>
#include <state.h>

namespace OTest2 {

StateOrdinary::StateOrdinary(
    const Context& context_) {

}

StateOrdinary::~StateOrdinary() {

}

void StateOrdinary::scheduleRun(
    const Context& context_,
    StatePtr this_ptr_) {
  auto so_(this_ptr_.staticCast<StateOrdinary>());
  assert(so_.get() == this);

  /* -- schedule the commands */
  context_.command_stack->pushCommand(
      std::make_shared<CmdRunState>(so_, false, -1));
}

} /* namespace OTest2 */

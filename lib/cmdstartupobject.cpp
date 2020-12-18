/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2
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

#include <cmdstartupobject.h>

#include <assert.h>
#include <memory>

#include <cmdnextobject.h>
#include <cmdteardownobject.h>
#include <commandstack.h>
#include <context.h>
#include <objectscenario.h>
#include <semanticstack.h>

namespace OTest2 {

CmdStartUpObject::CmdStartUpObject(
    ObjectScenarioPtr object_,
    ScenarioPtr scenario_,
    ObjectPtr parent_,
    int index_) :
  object(object_),
  scenario(scenario_),
  parent(parent_),
  index(index_) {
  assert(object != nullptr && scenario != nullptr && index >= 0);
}

CmdStartUpObject::~CmdStartUpObject() {

}

void CmdStartUpObject::run(
    const Context& context_) {
  if(object->startUpObject(context_, index)) {
    /* -- check result of the start-up function */
    if(context_.semantic_stack->top()) {
      /* -- the start-up function passed -> schedule next start-up function */
      context_.command_stack->pushCommand(
          std::make_shared<CmdStartUpObject>(object, scenario, parent, index + 1));
    }
    else {
      /* -- the start-up function failed -> schedule clean-up of previously
       *    invoked functions. */
      if(index > 0)
        context_.command_stack->pushCommand(
            std::make_shared<CmdTearDownObject>(object, scenario, index - 1));
    }
  }
  else {
    /* -- Cll start-up functions has passed. Continue. */

    /* -- schedule cleaning up */
    if(index > 0)
      context_.command_stack->pushCommand(
          std::make_shared<CmdTearDownObject>(object, scenario, index - 1));

    /* -- schedule run of the object body */
    object->scheduleBody(context_, scenario, object);
  }
}

} /* -- namespace OTest2 */

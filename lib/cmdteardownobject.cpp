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

#include <cmdteardownobject.h>

#include <assert.h>
#include <memory>

#include <commandstack.h>
#include <context.h>
#include <objectscenario.h>

namespace OTest2 {

CmdTearDownObject::CmdTearDownObject(
    ObjectScenarioPtr object_,
    ScenarioPtr scenario_,
    int index_) :
  object(object_),
  scenario(scenario_),
  index(index_) {
  assert(object != nullptr && scenario != nullptr && index >= 0);

}

CmdTearDownObject::~CmdTearDownObject() {

}

void CmdTearDownObject::run(
    const Context& context_) {
  /* -- run the tear-down function */
  object->tearDownObject(context_, index);

  /* -- schedule next tear-down function or finish the suite */
  if(index > 0)
    context_.command_stack->pushCommand(
        std::make_shared<CmdTearDownObject>(object, scenario, index - 1));
}

} /* -- namespace OTest2 */

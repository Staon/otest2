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
#include <cmdnextobject.h>

#include <assert.h>
#include <utility>

#include <cmdrepeatobject.h>
#include <commandstack.h>
#include <context.h>
#include <objectrepeater.h>
#include <scenario.h>
#include <scenarioiter.h>

namespace OTest2 {

CmdNextObject::CmdNextObject(
    ScenarioIterPtr scenario_iter_,
    ObjectPtr parent_) :
  scenario_iter(scenario_iter_),
  parent(parent_) {
  assert(scenario_iter != nullptr);

}

CmdNextObject::~CmdNextObject() {

}

void CmdNextObject::run(
    const Context& context_) {
  /* -- if the iterator still points a valid object, run it */
  if(scenario_iter->isValid()) {
    /* -- get the pointed scenario object */
    ScenarioPtr object_scenario_(scenario_iter->getScenario());

    /* -- schedule run of next testing object */
    scenario_iter->next();
    context_.command_stack->pushCommand(
        std::make_shared<CmdNextObject>(scenario_iter, parent));

    /* -- create the repeater object */
    auto repeater_(object_scenario_->createRepeater(context_));

    /* -- schedule the repeater for run */
    context_.command_stack->pushCommand(
        std::make_shared<CmdRepeatObject>(
            object_scenario_, repeater_.second, repeater_.first, parent));
  }
}

} /* -- namespace OTest2 */

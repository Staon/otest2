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
#include <cmdrepeatobject.h>

#include <assert.h>

#include <cmdleaveobject.h>
#include <cmdstartupobject.h>
#include <commandstack.h>
#include <context.h>
#include <objectscenario.h>
#include <objectpath.h>
#include <objectrepeater.h>
#include "runcode.h"
#include <scenario.h>
#include <semanticstack.h>

namespace OTest2 {

CmdRepeatObject::CmdRepeatObject(
    ScenarioPtr scenario_,
    ObjectRepeaterPtr repeater_,
    const std::string& name_,
    ObjectPtr parent_) :
  scenario(scenario_),
  repeater(repeater_),
  name(name_),
  parent(parent_) {
  assert(scenario != nullptr && repeater != nullptr && !name.empty());

}

CmdRepeatObject::~CmdRepeatObject() {

}

void CmdRepeatObject::run(
    const Context& context_) {
  if(repeater->isNextRun(context_)) {
    /* -- decorate object's name for next run */
    std::string decorated_name_(repeater->transformName(context_, name));

    /* -- schedule myself for next run */
    context_.command_stack->pushCommand(
        std::make_shared<CmdRepeatObject>(scenario, repeater, name, parent));

    /* -- prepare stack-frame of the object - object's result and path. */
    context_.object_path->pushName(decorated_name_);
    context_.semantic_stack->push(true);

    /* -- report entering of the suite */
    scenario->reportEntering(context_, decorated_name_);

    /* -- schedule finishing of the suite */
    context_.command_stack->pushCommand(
        std::make_shared<CmdLeaveObject>(scenario, decorated_name_));

    /* -- The constructor method of the suite may throw and exception.
     *    So I do the creation in a protected environment. */
    runUserCode(context_, [&](const Context& context_) {
      ObjectScenarioPtr object_(
          repeater->createObject(context_, decorated_name_, parent));
      context_.command_stack->pushCommand(
          std::make_shared<CmdStartUpObject>(object_, scenario, parent, 0));
    });
  }
}

} /* -- namespace OTest2 */

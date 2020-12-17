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

#include <cmdleaveobject.h>

#include <assert.h>

#include <context.h>
#include <objectpath.h>
#include <scenario.h>
#include <semanticstack.h>

namespace OTest2 {

CmdLeaveObject::CmdLeaveObject(
    ScenarioPtr scenario_,
    const std::string& decorated_name_) :
  scenario(scenario_),
  decorated_name(decorated_name_) {
  assert(scenario != nullptr && !decorated_name.empty());

}

CmdLeaveObject::~CmdLeaveObject() {

}

void CmdLeaveObject::run(
    const Context& context_) {
  /* -- report finishing of the object */
  scenario->reportLeaving(context_, decorated_name, context_.semantic_stack->top());

  /* -- finish object's stack-frame */
  context_.object_path->popName();
  context_.semantic_stack->popAnd();
}

} /* -- namespace OTest2 */

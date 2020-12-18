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

#include <testroot.h>

#include <assert.h>
#include <memory>

#include <cmdnextobject.h>
#include <commandstack.h>
#include <context.h>
#include <scenario.h>

namespace OTest2 {

TestRoot::TestRoot(
    const std::string& name_) :
  name(name_) {
  assert(!name.empty());

}

TestRoot::~TestRoot() {

}

std::string TestRoot::getName() const {
  return name;
}

bool TestRoot::startUpObject(
    const Context& context_,
    int index_) {
  return false; /* -- no start-up functions */
}

void TestRoot::scheduleBody(
    const Context& context_,
    ScenarioPtr scenario_,
    ObjectPtr me_) {
  ScenarioIterPtr children_(scenario_->getChildren());
  context_.command_stack->pushCommand(
      std::make_shared<CmdNextObject>(children_, me_));
}

void TestRoot::tearDownObject(
    const Context& context_,
    int index_) {
  assert(index_ == 0);
  /* -- no tear-down functions */
}

} /* -- namespace OTest2 */

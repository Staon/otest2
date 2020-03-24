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

#include <cmdstarttest.h>

#include <assert.h>
#include <memory>

#include <cmdnextsuite.h>
#include <cmdstoptest.h>
#include <commandptr.h>
#include <commandstack.h>
#include <context.h>
#include <objectpath.h>
#include <reporter.h>

namespace OTest2 {

CmdStartTest::CmdStartTest(
    const std::string& name_,
    Registry* registry_) :
  name(name_),
  registry(registry_) {
  assert(registry != nullptr);

}

CmdStartTest::~CmdStartTest() {

}

void CmdStartTest::run(
    const Context& context_) {
  /* -- report the test */
  context_.object_path->pushName(name);
  context_.reporter->enterTest(context_, name);

  /* -- schedule the commands */
  context_.command_stack->pushCommand(
      std::make_shared<CmdStopTest>(name));
  context_.command_stack->pushCommand(
      std::make_shared<CmdNextSuite>(registry, 0));
}

} /* -- namespace OTest2 */

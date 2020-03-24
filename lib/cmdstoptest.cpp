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

#include <cmdstoptest.h>

#include <context.h>
#include <objectpath.h>
#include <reporter.h>
#include <semanticstack.h>

namespace OTest2 {

CmdStopTest::CmdStopTest(
    const std::string& name_) :
  name(name_) {

}

CmdStopTest::~CmdStopTest() {

}

void CmdStopTest::run(
    const Context& context_) {
  /* -- report the end of the test */
  context_.reporter->leaveTest(
      context_, name, context_.semantic_stack->top());
  context_.object_path->popName();

  /* -- note: don't touch the semantic stack. There is the result of the entire
   *    test at the top. */
}

} /* -- namespace OTest2 */

/*
 * Copyright (C) 2020 Ondrej Starek
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

#include <cmdteardowncase.h>

#include <assert.h>
#include <memory>

#include <caseordinary.h>
#include <commandptr.h>
#include <commandstack.h>
#include <context.h>

namespace OTest2 {

CmdTearDownCase::CmdTearDownCase(
    CaseOrdinaryPtr testcase_,
    int index_) :
  testcase(testcase_),
  index(index_) {
  assert(!testcase.isNull() && index >= 0);

}

CmdTearDownCase::~CmdTearDownCase() {

}

void CmdTearDownCase::run(
    const Context& context_) {
  /* -- run the tear-down function */
  testcase->tearDownCase(context_, index);

  /* -- schedule run of next tear-down function */
  if(index > 0)
    context_.command_stack->pushCommand(
        std::make_shared<CmdTearDownCase>(testcase, index - 1));
}

} /* -- namespace OTest2 */

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

#include <cmdstartupsuite.h>

#include <assert.h>
#include <memory>

#include <cmdnextcase.h>
#include <cmdteardownsuite.h>
#include <commandptr.h>
#include <commandstack.h>
#include <context.h>
#include <semanticstack.h>
#include <suiteordinary.h>

namespace OTest2 {

CmdStartUpSuite::CmdStartUpSuite(
    SuiteOrdinaryPtr suite_,
    int index_) :
  suite(suite_),
  index(index_) {
  assert(!suite.isNull() && index >= 0);

}

CmdStartUpSuite::~CmdStartUpSuite() {

}

void CmdStartUpSuite::run(
    const Context& context_) {
  if(suite->startUpSuite(context_, index)) {
    /* -- check result of the start-up function */
    if(context_.semantic_stack->top()) {
      /* -- the start-up function passed -> schedule next start-up function */
      context_.command_stack->pushCommand(
          std::make_shared<CmdStartUpSuite>(suite, index + 1));
    }
    else {
      /* -- the start-up function failed -> schedule clean-up of previously
       *    invoked functions. */
      if(index > 0)
        context_.command_stack->pushCommand(
            std::make_shared<CmdTearDownSuite>(suite, index - 1));
    }
  }
  else {
    /* -- all start-up functions are finished, schedule body of the suite
     *    and clean-up of the suite */
    if(index > 0)
      context_.command_stack->pushCommand(
          std::make_shared<CmdTearDownSuite>(suite, index - 1));
    context_.command_stack->pushCommand(std::make_shared<CmdNextCase>(suite, 0));
  }
}

} /* -- namespace OTest2 */

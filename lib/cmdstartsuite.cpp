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

#include <cmdstartsuite.h>

#include <assert.h>
#include <memory>

#include <cmddestroysuite.h>
#include <cmdnextcase.h>
#include <commandstack.h>
#include <context.h>
#include <objectpath.h>
#include <reporter.h>
#include <semanticstack.h>
#include <suiteordinary.h>

namespace OTest2 {

CmdStartSuite::CmdStartSuite(
    SuiteOrdinaryPtr suite_) :
  suite(suite_) {
  assert(!suite.isNull());

}

CmdStartSuite::~CmdStartSuite() {

}

void CmdStartSuite::run(
    const Context& context_) {
  /* -- report start of the suite */
  context_.object_path->pushName(suite->getName());
  context_.reporter->enterSuite(context_, suite->getName());

  /* -- prepare suite's result */
  context_.semantic_stack->push(true);

  /* -- begin initialization of the suite */
  suite->startUpSuite(context_);
  if(!context_.semantic_stack->top()) {
    /* -- initialization failed */
    context_.reporter->leaveSuite(context_, suite->getName(), false);
    context_.object_path->popName();
    context_.semantic_stack->popAnd();
  }
  else {
    /* -- initialization passed - push the clean-up command */
    context_.command_stack->pushCommand(
        std::make_shared<CmdDestroySuite>(suite));

    /* -- run the suite */
    context_.command_stack->pushCommand(
        std::make_shared<CmdNextCase>(suite, 0));
  }
}

} /* namespace OTest2 */

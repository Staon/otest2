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

#include <cmdrepeatcase.h>

#include <assert.h>
#include <memory>
#include <string>

#include <caserepeater.h>
#include <cmdleavecase.h>
#include <commandstack.h>
#include <context.h>
#include <objectpath.h>
#include <reporter.h>
#include "runcode.h"
#include <runnerfilter.h>
#include <semanticstack.h>
#include <case.h>

namespace OTest2 {

CmdRepeatCase::CmdRepeatCase(
    CaseRepeaterPtr repeater_,
    const std::string& suite_name_,
    const std::string& case_name_) :
  repeater(repeater_),
  suite_name(suite_name_),
  case_name(case_name_) {
  assert(repeater != nullptr && !suite_name.empty() && !case_name.empty());

}

CmdRepeatCase::~CmdRepeatCase() {

}

void CmdRepeatCase::run(
    const Context& context_) {
  if(repeater->isNextRun(context_)) {
    /* -- next run is expected, decorate the case name */
    std::string decorated_name_(repeater->transformName(context_, case_name));

    /* -- prepare myself for next run */
    context_.command_stack->pushCommand(
        std::make_shared<CmdRepeatCase>(repeater, suite_name, case_name));

    /* -- prepare the stack frame */
    context_.semantic_stack->push(true);
    context_.object_path->pushName(decorated_name_);

    /* -- report entering of the test case */
    context_.reporter->enterCase(context_, decorated_name_);

    /* -- schedule finishing of the test case */
    context_.command_stack->pushCommand(std::make_shared<CmdLeaveCase>());

    /* -- Create and schedule the test case. As the constructor method
     *    can throw an exception the code is run in the protected
     *    environment. */
    runUserCode(context_, [&](const Context& context_){
      CasePtr testcase_(repeater->createCase(context_, decorated_name_));
      testcase_->scheduleRun(context_, testcase_);
    });
  }
}

} /* -- namespace OTest2 */

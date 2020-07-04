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

#include <cmdrepeatsuite.h>

#include <assert.h>
#include <memory>

#include <cmdleavesuite.h>
#include <context.h>
#include <commandstack.h>
#include <objectpath.h>
#include <reporter.h>
#include <runnerfilter.h>
#include "runcode.h"
#include <semanticstack.h>
#include <suite.h>
#include <suiteptr.h>
#include <suiterepeater.h>

namespace OTest2 {

CmdRepeatSuite::CmdRepeatSuite(
    SuiteRepeaterPtr repeater_,
    const std::string& suite_name_) :
  repeater(repeater_),
  suite_name(suite_name_) {
  assert(repeater != nullptr && !suite_name.empty());

}

CmdRepeatSuite::~CmdRepeatSuite() {

}

void CmdRepeatSuite::run(
    const Context& context_) {
  if(repeater->isNextRun(context_)) {
    /* -- decorate suite name for next run */
    std::string decorated_name_(repeater->transformName(context_, suite_name));

    /* -- prepare myself for next suite run */
    context_.command_stack->pushCommand(
        std::make_shared<CmdRepeatSuite>(repeater, suite_name));

    /* -- prepare stack-frame of the suite - suite's result and suite's
     *    path. */
    context_.object_path->pushName(decorated_name_);
    context_.semantic_stack->push(true);

    /* -- report entering of the suite */
    context_.reporter->enterSuite(context_, decorated_name_);

    /* -- schedule finishing of the suite */
    context_.command_stack->pushCommand(std::make_shared<CmdLeaveSuite>());

    /* -- The constructor method of the suite may throw and exception.
     *    So I do the creation in a protected environment. */
    runUserCode(context_, [&](const Context& context_) {
      SuitePtr suite_(repeater->createSuite(context_, decorated_name_));
      suite_->scheduleRun(context_, suite_);
    });
  }
}

} /* -- namespace OTest2 */

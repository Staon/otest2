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

#include <cmdnextcase.h>

#include <assert.h>
#include <memory>
#include <string>

#include <case.h>
#include <casefactory.h>
#include <casefactoryptr.h>
#include <caseptr.h>
#include <cmdstartcase.h>
#include <commandptr.h>
#include <commandstack.h>
#include <context.h>
#include <runnerfilter.h>
#include <suiteordinary.h>

namespace OTest2 {

CmdNextCase::CmdNextCase(
    SuiteOrdinaryPtr suite_,
    int current_) :
  suite(suite_),
  current(current_) {
  assert(!suite.isNull() && current >= 0);

}

CmdNextCase::~CmdNextCase() {

}

void CmdNextCase::run(
    const Context& context_) {
  /* -- get the case factory */
  std::string case_name_;
  CaseFactoryPtr factory_(suite->getCase(context_, current, &case_name_));
  if(factory_ != nullptr) {
    /* -- There is a case at the index. Schedule run of next one. */
    context_.command_stack->pushCommand(
        std::make_shared<CmdNextCase>(suite, current + 1));

    /* -- Schedule run of the test case */
    if(!context_.runner_filter->filterCase(suite->getName(), case_name_)) {
      CasePtr testcase_(factory_->createCase(context_));
      testcase_->scheduleRun(context_, testcase_);
    }
  }
}

} /* namespace OTest2 */

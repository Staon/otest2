/*
 * Copyright (C) 2019 Ondrej Starek
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

#include <cmdstate.h>

#include <assert.h>
#include <sstream>

#include <caseordinary.h>
#include <context.h>
#include <internalerror.h>
#include <reporter.h>
#include <state.h>

namespace OTest2 {

CmdState::CmdState(
    CaseOrdinaryPtr testcase_,
    const std::string& name_,
    int delay_) :
  testcase(testcase_),
  name(name_),
  delay(delay_) {
  assert(testcase != nullptr && !name_.empty() && delay_ >= 0);

}

CmdState::~CmdState() {

}

void CmdState::run(
    const Context& context_) {
  StatePtr state_(testcase->getState(name));
  if(state_ != nullptr) {
    state_->scheduleRun(context_, testcase, state_, true, delay);
  }
  else {
    std::ostringstream os_;
    os_ << "invalid state name '" << name << "'";
    internalError(context_, os_.str());
  }
}

} /* namespace OTest2 */

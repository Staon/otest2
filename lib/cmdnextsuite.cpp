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

#include <cmdnextsuite.h>

#include <assert.h>
#include <memory>
#include <string>

#include <commandstack.h>
#include <context.h>
#include <objectpath.h>
#include <registry.h>
#include "runcode.h"
#include <runnerfilter.h>
#include <suitefactory.h>
#include <suitefactoryptr.h>
#include <suite.h>
#include <suiteptr.h>

namespace OTest2 {

CmdNextSuite::CmdNextSuite(
    Registry* registry_,
    int current_) :
  registry(registry_),
  current(current_) {
  assert(registry != nullptr && current >= 0);

}

CmdNextSuite::~CmdNextSuite() {

}

void CmdNextSuite::run(
    const Context& context_) {
  /* -- remove previous suite from the object path */
  if(current > 0)
    context_.object_path->popName();

  std::string suite_name_;
  SuiteFactoryPtr factory_(registry->getSuite(current, &suite_name_));
  if(factory_ != nullptr) {
    /* -- schedule run of next suite */
    context_.command_stack->pushCommand(
        std::make_shared<CmdNextSuite>(registry, current + 1));

    /* -- push the suite into the object path -> an exception fired from
     *    the suite constructor will be reported at correct path. */
    context_.object_path->pushName(suite_name_);

    /* -- create and run the suite if it's not filtered */
    runUserCode(context_, [&](const Context& context_){
      if(!context_.runner_filter->filterSuite(suite_name_)) {
        SuitePtr suite_(factory_->createSuite(context_));
        suite_->scheduleRun(context_, suite_);
      }
    });
  }
}

} /* -- namespace OTest2 */

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

#include <cmdpoptags.h>
#include <cmdrepeatsuite.h>
#include <commandstack.h>
#include <context.h>
#include <registry.h>
#include <runnerfilter.h>
#include <suitefactory.h>
#include <suiterepeater.h>
#include <tagfilter.h>
#include <tags.h>
#include <tagsstack.h>

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
  /* -- create the suite */
  std::string suite_name_;
  SuiteFactoryPtr factory_(registry->getSuite(current, &suite_name_));
  if(factory_ != nullptr) {
    /* -- schedule run of next suite */
    context_.command_stack->pushCommand(
        std::make_shared<CmdNextSuite>(registry, current + 1));

    /* -- merge tags with parent ones */
    context_.command_stack->pushCommand(std::make_shared<CmdPopTags>());
    context_.tags_stack->pushTags(true, factory_->getSuiteTags());

    /* -- create and schedule the suite if it's not filtered */
    if(!context_.runner_filter->filterSuite(suite_name_)
       && !context_.tag_filter->filterObject(*context_.tags_stack)) {
      /* -- schedule the repeater command */
      SuiteRepeaterPtr repeater_(factory_->createSuite(context_));
      context_.command_stack->pushCommand(
          std::make_shared<CmdRepeatSuite>(repeater_, suite_name_));
    }
  }
}

} /* -- namespace OTest2 */

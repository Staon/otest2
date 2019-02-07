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

#include <runnerordinary.h>

#include <assert.h>
#include <string>

#include <cmdnextsuite.h>
#include <cmdstarttest.h>
#include <cmdstoptest.h>
#include <commandptr.h>
#include <commandstack.h>
#include <context.h>
#include <reporter.h>
#include <semanticstack.h>
#include <utils.h>

namespace OTest2 {

struct RunnerOrdinary::Impl {
  public:
    RunnerOrdinary* owner;

    CommandStack command_stack;
    SemanticStack semantic_stack;
    Registry* registry;
    std::string name;
    Context context;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        RunnerOrdinary* owner_,
        Reporter* reporter_,
        Registry* registry_,
        RunnerFilter* runner_filter_,
        const std::string& name_);
    ~Impl();
};

RunnerOrdinary::Impl::Impl(
    RunnerOrdinary* owner_,
    Reporter* reporter_,
    Registry* registry_,
    RunnerFilter* runner_filter_,
    const std::string& name_) :
  owner(owner_),
  command_stack(),
  semantic_stack(),
  registry(registry_),
  name(name_),
  context(&command_stack, &semantic_stack, reporter_, runner_filter_) {
  assert(registry != nullptr);
  assert(context.reporter != nullptr);
  assert(context.runner_filter != nullptr);

  /* -- prepare start of the test */
  command_stack.pushCommand(std::make_shared<CmdStartTest>(name, registry));
  semantic_stack.push(true); /* -- test passes by default */
}

RunnerOrdinary::Impl::~Impl() {

}

RunnerOrdinary::RunnerOrdinary(
    Reporter* reporter_,
    Registry* registry_,
    RunnerFilter* runner_filter_,
    const std::string& name_) :
  pimpl(new Impl(this, reporter_, registry_, runner_filter_, name_)) {

}

RunnerOrdinary::~RunnerOrdinary() {
  odelete(pimpl);
}

int RunnerOrdinary::runNext() {
  bool first_command_(true);
  while(!pimpl->command_stack.empty()) {
    CommandPtr cmd_(pimpl->command_stack.topCommand());

    /* -- check whether we should get back into the main loop */
    int delay_(0);
    if(!first_command_ && cmd_->shouldWait(pimpl->context, delay_)) {
      assert(delay_ >= 0);
      return delay_;
    }

    /* -- run the command */
    first_command_ = false;
    pimpl->command_stack.popCommand();
    cmd_->run(pimpl->context);
  }

  return -1; /* -- no other work */
}

} /* namespace OTest2 */

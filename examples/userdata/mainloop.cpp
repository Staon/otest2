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

#include "mainloop.h"

#include <event2/event.h>
#include <iostream>
#include <otest2/runner.h>
#include <otest2/utils.h>
#include <sys/time.h>

namespace OTest2 {

namespace Examples {

struct MainLoop::Impl {
    event_base* base;
    event* ev;
    ::OTest2::Runner* runner;
    bool result;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    Impl();
    ~Impl();

    void scheduleTimer(
        int delay_);
    static void timerCallback(
        evutil_socket_t,
        short,
        void* udata_);

};

MainLoop::Impl::Impl() :
  base(event_base_new()),
  ev(event_new(base, -1, 0, timerCallback, this)),
  runner(nullptr),
  result(true) {

}

MainLoop::Impl::~Impl() {
  event_free(ev);
  event_base_free(base);
}

void MainLoop::Impl::scheduleTimer(
    int delay_) {
  struct timeval period_ = {delay_ / 1000, (delay_ % 1000) * 1000000};
  event_add(ev, &period_);
}

void MainLoop::Impl::timerCallback(
    evutil_socket_t,
    short,
    void* udata_) {
  Impl* self_(static_cast<Impl*>(udata_));

  ::OTest2::RunnerResult result_(self_->runner->runNext());
  if(!result_.isFinished()) {
    /* -- The test is not finished yet, schedule next test step. */
    self_->scheduleTimer(result_.getDelayMS());
  }
  else {
    /* -- The test is finished. Stop the main loop and return the test
     *    result. */
    event_base_loopbreak(self_->base);
    self_->result = result_.getResult();
  }
}

MainLoop::MainLoop() :
  pimpl(new Impl) {

}

MainLoop::~MainLoop() {
  ::OTest2::odelete(pimpl);
}

bool MainLoop::runTest(
    ::OTest2::Runner* runner_) {
  pimpl->runner = runner_;
  pimpl->result = false;
  pimpl->scheduleTimer(0);  /* -- first waking-up */

  /* -- enter the main loop */
  event_base_dispatch(pimpl->base);

  return pimpl->result;
}

void MainLoop::doSomethingUseful(
    const std::string& message_) {
  std::cout << message_ << std::endl;
}

} /* -- namespace Examples */

} /* -- namespace OTest2 */

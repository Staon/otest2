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

#include <event2/event.h>
#include <otest2/dfltenvironment.h>
#include <otest2/runner.h>
#include <sys/time.h>

namespace {

struct Loop {
    event_base* base;
    event* ev;
    ::OTest2::Runner* runner;
    bool result;
};

void scheduleTimer(
    Loop* loop_,
    int delay_) {
  struct timeval period_ = {delay_ / 1000, (delay_ % 1000) * 1000000};
  event_add(loop_->ev, &period_);
}

void timerCallback(
    evutil_socket_t,
    short,
    void* udata_) {
  Loop* loop_(static_cast<Loop*>(udata_));

  ::OTest2::RunnerResult result_(loop_->runner->runNext());
  if(!result_.isFinished()) {
    /* -- The test is not finished yet, schedule next test step. */
    scheduleTimer(loop_, result_.getDelayMS());
  }
  else {
    /* -- The test is finished. Stop the main loop and return the test
     *    result. */
    event_base_loopbreak(loop_->base);
    loop_->result = result_.getResult();
  }
}

} /* -- namespace */

int main(
    int argc_,
    char* argv_[]) {
  /* -- Prepare the testing environment. The default environment offered
   *    by the OTest2 framework is used. */
  ::OTest2::DfltEnvironment environment_(argc_, argv_);

  /* --Initialize the main loop and schedule start of the test. */
  Loop loop_;
  loop_.base = event_base_new();
  loop_.ev = event_new(loop_.base, -1, 0, timerCallback, &loop_);
  loop_.runner = &environment_.getRunner();
  loop_.result = false;
  scheduleTimer(&loop_, 0);  /* -- first waking-up */

  /* -- enter the main loop */
  event_base_dispatch(loop_.base);

  /* -- clean up the main loop */
  event_free(loop_.ev);
  event_base_free(loop_.base);

  return loop_.result ? 0 : 1;
}

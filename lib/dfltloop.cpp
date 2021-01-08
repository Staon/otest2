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
#include <dfltloop.h>

#include <chrono>
#include <thread>

#include <runner.h>

namespace OTest2 {

int defaultMainLoop(
    Runner& runner_) {
  ::OTest2::RunnerResult result_;
  while(true) {
    /* -- do the test step */
    result_ = runner_.runNext();
    if(result_.isFinished())
      break;

    /* -- wait for next step */
    const std::chrono::milliseconds delay_(result_.getDelayMS());
    if(delay_ > std::chrono::milliseconds(0))
      std::this_thread::sleep_for(delay_);
  }

  if(result_.getResult())
    return 0;
  else
    return 1;
}

} /* -- namespace OTest2 */

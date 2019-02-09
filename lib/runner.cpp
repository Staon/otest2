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

#include <runner.h>

#include <algorithm>
#include <assert.h>

namespace OTest2 {

RunnerResult::RunnerResult() :
  running(false),
  result(false),
  delay_ms(-1) {

}

RunnerResult::RunnerResult(
    bool running_,
    bool result_,
    int delay_ms_) :
  running(running_),
  result(result_),
  delay_ms(delay_ms_) {
  assert((running && delay_ms >= 0) || (!running_));

}

RunnerResult::RunnerResult(
    const RunnerResult& src_) :
  running(src_.running),
  result(src_.result),
  delay_ms(src_.delay_ms) {

}

RunnerResult::~RunnerResult() {

}

void RunnerResult::swap(
    RunnerResult& r2_) noexcept {
  std::swap(running, r2_.running);
  std::swap(result, r2_.result);
  std::swap(delay_ms, r2_.delay_ms);
}

RunnerResult& RunnerResult::operator = (
    const RunnerResult& src_) {
  RunnerResult tmp_(src_);
  swap(tmp_);
  return *this;
}

bool RunnerResult::isFinished() const {
  return !running;
}

int RunnerResult::getDelayMS() const {
  assert(running);
  return delay_ms;
}

bool RunnerResult::getResult() const {
  assert(!running);
  return result;
}

Runner::Runner() {

}

Runner::~Runner() {

}

} /* namespace OTest2 */

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

#include "runtime.h"

#include <iostream>
#include <unistd.h>

#include <otest2/registry.h>

namespace OTest2 {

namespace Test {

Runtime::Runtime(
    const std::string& suite_name_,
    const std::string& case_name_) :
  time_source(),
  exc_catcher(),
  reporter(),
  runner_filter(suite_name_, case_name_),
  test_mark_factory(),
  test_mark_storage(),
  runner(
      &time_source,
      &exc_catcher,
      &reporter,
      &Registry::instance("selftest"),
      &runner_filter,
      &test_mark_factory,
      test_mark_storage.get(),
      "selftest") {

}

Runtime::Runtime(
    const std::string& suite_name_,
    const std::string& case_name_,
    const std::string& regression_file_) :
  exc_catcher(),
  reporter(),
  runner_filter(suite_name_, case_name_),
  test_mark_factory(),
  test_mark_storage(new TestMarkStorage(&test_mark_factory, regression_file_)),
  runner(
      &time_source,
      &exc_catcher,
      &reporter,
      &Registry::instance("selftest"),
      &runner_filter,
      &test_mark_factory,
      test_mark_storage.get(),
      "selftest") {

}

Runtime::~Runtime() {

}

bool Runtime::runTheTest() {
  RunnerResult result_;
  while(true) {
    result_ = runner.runNext();
    if(result_.isFinished())
      break;
    reporter.reportDelay(result_.getDelayMS());
  };

  return result_.getResult();
}

} /* namespace Test */

} /* namespace OTest2 */

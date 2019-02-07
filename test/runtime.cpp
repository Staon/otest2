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

#include <otest2/registry.h>
#include "selftests/linkermark.h"

namespace OTest2 {

namespace Test {

Runtime::Runtime(
    const std::string& suite_name_,
    const std::string& case_name_) :
  reporter(&std::cout),
  runner_filter(suite_name_, case_name_),
  runner(
      &reporter,
      &Registry::instance("selftest"),
      &runner_filter,
      SelfTest::SELF_TEST_NAME) {

}

Runtime::~Runtime() {

}

bool Runtime::runTheTest() {
  int delay_(0);
  do {
    delay_ = runner.runNext();
  } while(delay_ >= 0);

  return true;
}

} /* namespace Test */

} /* namespace OTest2 */

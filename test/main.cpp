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

#include <iostream>
#include <otest2/registry.h>
#include <otest2/reporterconsole.h>
#include <otest2/runnerordinary.h>
#include <string>
#include "../include/otest2/runnerfilterentire.h"

int main(
    int argc_,
    char* argv_[]) {
  ::OTest2::ReporterConsole reporter_(&std::cout);
  ::OTest2::RunnerFilterEntire runner_filter_;
  ::OTest2::RunnerOrdinary runner_(
      &reporter_,
      &::OTest2::Registry::instance("default"),
      &runner_filter_,
      "selftest");
  int delay_(0);
  do {
    delay_ = runner_.runNext();
  } while(delay_ >= 0);

  return 0;
}

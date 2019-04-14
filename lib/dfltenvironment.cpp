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
#include <dfltenvironment.h>

#include <iostream>
#include <memory>
#include <string>

#include <exccatcherordinary.h>
#include <registry.h>
#include <reporterconsole.h>
#include <runnerfilterentire.h>
#include <runnerordinary.h>
#include <utils.h>

namespace OTest2 {

struct DfltEnvironment::Impl {
    std::unique_ptr<ExcCatcher> exc_catcher;
    std::unique_ptr<Reporter> reporter;
    std::unique_ptr<RunnerFilter> filter;
    std::unique_ptr<Runner> runner;
};

DfltEnvironment::DfltEnvironment(
    int argc_,
    char* argv_[]) :
  pimpl(new Impl) {

  /* -- TODO: parse command line options */

  pimpl->exc_catcher.reset(new ExcCatcherOrdinary);
  pimpl->reporter.reset(new ReporterConsole(&std::cout));
  pimpl->filter.reset(new RunnerFilterEntire);
  pimpl->runner.reset(new RunnerOrdinary(
      pimpl->exc_catcher.get(),
      pimpl->reporter.get(),
      &Registry::instance("default"),
      pimpl->filter.get(),
      argv_[0]));
}

DfltEnvironment::~DfltEnvironment() {
  odelete(pimpl);
}

Runner& DfltEnvironment::getRunner() {
  return *pimpl->runner;
}

} /* namespace OTest2 */

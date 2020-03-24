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

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <exccatcherordinary.h>
#include <registry.h>
#include <reporterconsole.h>
#include <reporterjunit.h>
#include <reportertee.h>
#include <runnerfilterentire.h>
#include <runnerfilterone.h>
#include <runnerordinary.h>
#include <testmarkfactory.h>
#include <testmarkstorage.h>
#include <timesourcesys.h>
#include <utils.h>

namespace OTest2 {

namespace {

void printHelpMessage(
    const char* binary_name_) {
  std::cout << "Usage: " << binary_name_ << " [options]" << std::endl;
  std::cout << std::endl;
  std::cout << "A collection of automatic tests run by the OTest2 framework" << std::endl;
  std::cout << std::endl;
  std::cout << "Options (all options are optional):" << std::endl;
  std::cout << "  -h       --help             Print this message." << std::endl;
  std::cout << "           --disable-console  Disable reporting into the console." << std::endl;
  std::cout << "  -v       --verbose          Make the console reporter verbose. This option" << std::endl;
  std::cout << "                              turns on printing of results of passed tests." << std::endl;
  std::cout << "  -j file  --junit=file       Print report into a file in the JUnit XML format." << std::endl;
  std::cout << "                              This option may be used several times. Several" << std::endl;
  std::cout << "                              files will then be written." << std::endl;
  std::cout << "  -r obj   --restrictive=obj  Run just only the specified testing object." << std::endl;
  std::cout << "                              'obj' is in the format '<suite>[::<case>]'." << std::endl;
  std::cout << "                              If just the suite name is got the whole suite" << std::endl;
  std::cout << "                              is run. A complete object path means running" << std::endl;
  std::cout << "                              of just one test case." << std::endl;
  std::cout << "  -m file  --regression=file  Path of the regression file. The default value" << std::endl;
  std::cout << "                              is 'regression.ot2tm' (stored in the working" << std::endl;
  std::cout << "                              directory)." << std::endl;
  std::cout << "  -t name  --test=name        Name of the test how it's reported. The default" << std::endl;
  std::cout << "                              value is the name of the test's binary." << std::endl;
}

} /* -- namespace */

struct DfltEnvironment::Impl {
    TimeSourceSys time_source;
    ExcCatcherOrdinary exc_catcher;
    std::vector<std::unique_ptr<Reporter>> reporters;
    ReporterTee reporter_root;
    std::unique_ptr<RunnerFilter> filter;
    TestMarkFactory test_mark_factory;
    std::unique_ptr<TestMarkStorage> test_mark_storage;
    std::unique_ptr<Runner> runner;
};

DfltEnvironment::DfltEnvironment(
    int argc_,
    char* argv_[]) :
  pimpl(new Impl) {

  bool console_reporter_(true);
  bool console_verbose_(false);
  std::string regression_file_("regression.ot2tm");

  /* -- Default test name is derived from the test binary as the last part
   *    of the path. */
  std::string test_name_(argv_[0]);
  int last_slash_(test_name_.find_last_of('/'));
  if(last_slash_ != std::string::npos)
    test_name_.erase(last_slash_);

  enum {
    DISABLE_CONSOLE_REPORTER = 1000,
    VERBOSE_CONSOLE,
    JUNIT_REPORTER,
    RESTRICTIVE_RUN,
    REGRESSION_FILE,
    TEST_NAME,
    HELP,
  };
  struct option long_options_[] = {
      {"disable-console", 0, nullptr, DISABLE_CONSOLE_REPORTER},
      {"verbose", 0, nullptr, VERBOSE_CONSOLE},
      {"junit", 1, nullptr, JUNIT_REPORTER},
      {"restrictive", 1, nullptr, RESTRICTIVE_RUN},
      {"regression", 1, nullptr, REGRESSION_FILE},
      {"test", 1, nullptr, TEST_NAME},
      {"help", 0, nullptr, HELP},
      {nullptr, 0, nullptr, 0},
  };
  int opt_;
  while((opt_ = getopt_long(argc_, argv_, "vj:r:m:t:h", long_options_, nullptr)) >= 0) {
    switch(opt_) {
      case DISABLE_CONSOLE_REPORTER:
        console_reporter_ = false;
        break;
      case 'v':
      case VERBOSE_CONSOLE:
        console_verbose_ = true;
        break;
      case 'j':
      case JUNIT_REPORTER:
        pimpl->reporters.emplace_back(new ReporterJUnit(optarg));
        pimpl->reporter_root.appendReporter(pimpl->reporters.back().get());
        break;
      case 'r':
      case RESTRICTIVE_RUN:
        pimpl->filter = RunnerFilterOne::fromPath(optarg);
        break;
      case 'm':
      case REGRESSION_FILE:
        regression_file_ = optarg;
        break;
      case 't':
      case TEST_NAME:
        test_name_ = optarg;
        break;
      case 'h':
      case HELP:
        printHelpMessage(argv_[0]);
        std::exit(0);
        break;
      case '?':
        printHelpMessage(argv_[0]);
        std::exit(1);
        break;
    }
  }

  /* -- create the console reporter if it's not disabled */
  if(console_reporter_) {
    pimpl->reporters.emplace_back(new ReporterConsole(&std::cout, console_verbose_));
    pimpl->reporter_root.appendReporter(pimpl->reporters.back().get());
  }

  /* -- create default runner filter - run all tests */
  if(pimpl->filter == nullptr)
    pimpl->filter.reset(new RunnerFilterEntire);

  /* -- create the test mark storage */
  pimpl->test_mark_storage.reset(
      new TestMarkStorage(&pimpl->test_mark_factory, regression_file_));

  /* -- finally, create the test runner */
  pimpl->runner.reset(new RunnerOrdinary(
      &pimpl->time_source,
      &pimpl->exc_catcher,
      &pimpl->reporter_root,
      &Registry::instance("default"),
      pimpl->filter.get(),
      &pimpl->test_mark_factory,
      pimpl->test_mark_storage.get(),
      test_name_));
}

DfltEnvironment::~DfltEnvironment() {
  odelete(pimpl);
}

Runner& DfltEnvironment::getRunner() {
  return *pimpl->runner;
}

} /* namespace OTest2 */

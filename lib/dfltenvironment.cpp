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

#include <assert.h>
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
#include <scenarioiterptr.h>
#include <tagfilter.h>
#include <testmarkfactory.h>
#include <testmarkstorage.h>
#include <timesourcesys.h>
#include <userdata.h>
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
  std::cout << "  -T expr  --tags=glob        Specification of the tag glob allowing to filter" << std::endl;
  std::cout << "                              testing objects from current run. The default" << std::endl;
  std::cout << "                              value runs all untagged objects." << std::endl;
  std::cout << std::endl;
}

std::string createDefaultTestName(
    const char* binname_) {
  /* -- Default test name is derived from the test binary as the last part
   *    of the path. */
  std::string test_name_(binname_);
  int last_slash_(test_name_.find_last_of('/'));
  if(last_slash_ != std::string::npos)
    test_name_.erase(0, last_slash_ + 1);
  return test_name_;
}

} /* -- namespace */

struct DfltEnvironment::Impl {
    /* -- the environment */
    TimeSourceSys time_source;
    ExcCatcherOrdinary exc_catcher_ordinary;
    ExcCatcher* exc_catcher;
    std::vector<std::unique_ptr<Reporter>> reporters;
    ReporterTee reporter_root;
    std::unique_ptr<RunnerFilter> filter;
    TestMarkFactory test_mark_factory;
    std::unique_ptr<TestMarkStorage> test_mark_storage;
    UserData user_data;
    std::unique_ptr<TagFilter> tag_filter;
    std::unique_ptr<Runner> runner;

    /* -- builder state */
    bool console_reporter;
    bool console_verbose;
    std::string regression_file;
    std::string test_name;

    explicit Impl(
        const std::string& test_name_);
};

DfltEnvironment::Impl::Impl(
    const std::string& test_name_) :
  time_source(),
  exc_catcher_ordinary(),
  exc_catcher(nullptr),
  reporters(),
  reporter_root(),
  filter(),
  test_mark_factory(),
  test_mark_storage(),
  user_data(),
  runner(),
  console_reporter(true),
  console_verbose(false),
  regression_file("regression.ot2tm"),
  test_name(test_name_) {

}

DfltEnvironment::DfltEnvironment(
    const std::string& testname_) :
  pimpl(new Impl(testname_)) {

}

DfltEnvironment::DfltEnvironment(
    int argc_,
    char* argv_[]) :
  pimpl(new Impl(createDefaultTestName(argv_[0]))) {

  enum {
    DISABLE_CONSOLE_REPORTER = 1000,
    VERBOSE_CONSOLE,
    JUNIT_REPORTER,
    RESTRICTIVE_RUN,
    REGRESSION_FILE,
    TEST_NAME,
    TAG_EXPRESSION,
    HELP,
  };
  struct option long_options_[] = {
      {"disable-console", 0, nullptr, DISABLE_CONSOLE_REPORTER},
      {"verbose", 0, nullptr, VERBOSE_CONSOLE},
      {"junit", 1, nullptr, JUNIT_REPORTER},
      {"restrictive", 1, nullptr, RESTRICTIVE_RUN},
      {"regression", 1, nullptr, REGRESSION_FILE},
      {"test", 1, nullptr, TEST_NAME},
      {"tags", 1, nullptr, TAG_EXPRESSION},
      {"help", 0, nullptr, HELP},
      {nullptr, 0, nullptr, 0},
  };
  int opt_;
  while((opt_ = getopt_long(argc_, argv_, "vj:r:m:t:T:h", long_options_, nullptr)) >= 0) {
    switch(opt_) {
      case DISABLE_CONSOLE_REPORTER:
        pimpl->console_reporter = false;
        break;
      case 'v':
      case VERBOSE_CONSOLE:
        pimpl->console_verbose = true;
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
        pimpl->regression_file = optarg;
        break;
      case 't':
      case TEST_NAME:
        pimpl->test_name = optarg;
        break;
      case 'T':
      case TAG_EXPRESSION:
        try {
          pimpl->tag_filter = ::OTest2::make_unique<TagFilter>(optarg);
        }
        catch(Exception& exc_) {
          std::cout << "invalid tag expression: " << exc_.reason() << std::endl;
          std::exit(2);
        }
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

}

DfltEnvironment::~DfltEnvironment() {
  odelete(pimpl);
}

void DfltEnvironment::addReporter(
    Reporter* reporter_) {
  assert(reporter_ != nullptr);

  pimpl->reporter_root.appendReporter(reporter_);
  pimpl->console_reporter = false;
}

void DfltEnvironment::setExceptionCatcher(
    ExcCatcher* catcher_) {
  assert(catcher_ != nullptr);

  pimpl->exc_catcher = catcher_;
}

UserData& DfltEnvironment::getUserData() noexcept {
  return pimpl->user_data;
}

Runner& DfltEnvironment::getRunner() {
  if(pimpl->runner == nullptr) {
    /* -- set default exception catcher */
    if(pimpl->exc_catcher == nullptr)
      pimpl->exc_catcher = &pimpl->exc_catcher_ordinary;

    /* -- create the console reporter if it's not disabled */
    if(pimpl->console_reporter) {
      pimpl->reporters.emplace_back(new ReporterConsole(
          &std::cout, pimpl->console_verbose));
      pimpl->reporter_root.appendReporter(pimpl->reporters.back().get());
    }

    /* -- create default runner filter - run all tests */
    if(pimpl->filter == nullptr)
      pimpl->filter = ::OTest2::make_unique<RunnerFilterEntire>();

    /* -- create the test mark storage */
    pimpl->test_mark_storage.reset(
        new TestMarkStorage(&pimpl->test_mark_factory, pimpl->regression_file));

    /* -- create the tag filter according to specified tag expression */
    if(pimpl->tag_filter == nullptr)
      pimpl->tag_filter = ::OTest2::make_unique<TagFilter>("[<empty>]");

    /* -- get the registry and set the test name */
    Registry& registry_(Registry::instance("default"));
    ScenarioIterPtr scenario_(registry_.getTests(*pimpl->filter, *pimpl->tag_filter));

    /* -- finally, create the test runner */
    pimpl->runner.reset(new RunnerOrdinary(
        &pimpl->time_source,
        pimpl->exc_catcher,
        &pimpl->reporter_root,
        &pimpl->test_mark_factory,
        pimpl->test_mark_storage.get(),
        &pimpl->user_data,
        scenario_));
  }
  return *pimpl->runner;
}

} /* namespace OTest2 */

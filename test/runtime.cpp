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
#include <sstream>
#include <unistd.h>

#include <otest2/registry.h>
#include <otest2/scenarioiterptr.h>
#include <otest2/utils.h>

namespace OTest2 {

namespace Test {

namespace {

std::unique_ptr<TestMarkStorage> createTestMarkStorage(
    TestMarkFactory* factory_,
    const std::string& regression_file_) {
  if(!regression_file_.empty())
    return ::OTest2::make_unique<TestMarkStorage>(factory_, regression_file_);
  else
    return std::unique_ptr<TestMarkStorage>();
}

ScenarioIterPtr prepareScenario(
    const std::string& domain_,
    const RunnerFilter& filter_) {
  Registry& registry_(Registry::instance(domain_));
  registry_.setTestName(domain_);
  return registry_.getTests(filter_);
}

std::string createTagGlob(
    const std::string& suite_name_,
    const std::string& case_name_) {
  std::ostringstream oss_;
  if(suite_name_.empty())
    oss_ << "**";
  else
    oss_ << suite_name_;
  oss_ << "::";
  if(case_name_.empty())
    oss_ << "**";
  else
    oss_ << case_name_;
  return oss_.str();
}

} /* -- namespace */

Runtime::ReportPaths Runtime::report_paths_mark;
Runtime::Tags Runtime::tags_mark;
Runtime::InternalCtor Runtime::internal_ctor;

Runtime::Runtime(
    const std::string& suite_name_,
    const std::string& case_name_) :
  Runtime(
      internal_ctor,
      createTagGlob(suite_name_, case_name_),
      nullptr,
      false,
      "") {

}

Runtime::Runtime(
    const std::string& suite_name_,
    const std::string& case_name_,
    const ReportPaths&) :
  Runtime(
      internal_ctor,
      createTagGlob(suite_name_, case_name_),
      nullptr,
      true,
      "") {

}

Runtime::Runtime(
    const std::string& suite_name_,
    const std::string& case_name_,
    const std::string& regression_file_) :
  Runtime(
      internal_ctor,
      createTagGlob(suite_name_, case_name_),
      nullptr,
      false,
      regression_file_) {

}

Runtime::Runtime(
    const Tags&,
    const std::string& tag_glob_) :
  Runtime(internal_ctor, tag_glob_, nullptr, false, "") {

}

Runtime::Runtime(
    const std::string& regression_file_,
    const Tags&,
    const std::string& tag_glob_) :
  Runtime(internal_ctor, tag_glob_, nullptr, false, regression_file_) {

}

Runtime::Runtime(
    const std::string& suite_name_,
    const std::string& case_name_,
    Reporter* reporter_) :
  Runtime(
      internal_ctor,
      createTagGlob(suite_name_, case_name_),
      reporter_,
      false,
      "") {

}

Runtime::Runtime(
    const Runtime::InternalCtor&,
    const std::string& tag_glob_,
    Reporter* reporter_,
    bool report_paths_,
    const std::string& regression_file_) :
  exc_catcher(),
  reporter(report_paths_),
  runner_filter(tag_glob_),
  test_mark_factory(),
  test_mark_storage(createTestMarkStorage(&test_mark_factory, regression_file_)),
  user_data(),
  runner(
      &time_source,
      &exc_catcher,
      (reporter_ == nullptr)?(&reporter):reporter_,
      &test_mark_factory,
      test_mark_storage.get(),
      &user_data,
      prepareScenario("selftest", runner_filter)) {
  /* -- prepare user data */
  user_data.setDatum("reporter_", &reporter);
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

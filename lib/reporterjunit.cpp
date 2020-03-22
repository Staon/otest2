/*
 * Copyright (C) 2020 Ondrej Starek
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

#include <reporterjunit.h>

#include <chrono>
#include <iomanip>
#include <pugixml.hpp>
#include <sstream>
#include <string>
#include <ctime>
#include <time.h>

#include <context.h>
#include <timesource.h>
#include <utils.h>

namespace OTest2 {

namespace {

std::string formatTimestamp(
    const TimeSource::time_point& time_) {
  std::time_t epoche_(std::chrono::system_clock::to_time_t(time_));
  struct tm broken_;
  localtime_r(&epoche_, &broken_);
  std::ostringstream oss_;
  oss_ << std::setfill('0') << std::dec << std::internal
      << std::setw(4) << (broken_.tm_year + 1900)
      << '-'
      << std::setw(2) << (broken_.tm_mon + 1)
      << '-'
      << std::setw(2) << broken_.tm_mday
      << 'T'
      << std::setw(2) << broken_.tm_hour
      << ':'
      << std::setw(2) << broken_.tm_min
      << ':'
      << std::setw(2) << broken_.tm_sec;
  return oss_.str();
}

std::string formatDuration(
    const TimeSource::time_point& start_,
    const TimeSource::time_point& end_) {
  std::chrono::duration<double> duration_(end_ - start_);
  std::ostringstream oss_;
  oss_ << std::setprecision(3) << std::fixed << duration_.count();
  return oss_.str();
}

} /* -- namespace */

struct ReporterJUnit::Impl {
    std::string filename;

    /* -- XML structure */
    ::pugi::xml_document doc;
    ::pugi::xml_node testsuites;

    /* -- reporter state */
    std::string testname;
    ::pugi::xml_node suite;
    TimeSource::time_point suite_start;
    int case_count;
    int case_failures;
    int case_errors;
    ::pugi::xml_node testcase;
    TimeSource::time_point case_start;
    bool case_first_failure;
    bool case_first_error;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& file_);
    ~Impl();
};

ReporterJUnit::Impl::Impl(
    const std::string& file_) :
  filename(file_),
  testname(),
  suite(),
  suite_start(),
  case_count(0),
  case_failures(0),
  case_errors(0),
  testcase(),
  case_start(),
  case_first_failure(false),
  case_first_error(false) {

  /* -- create the root 'testsuites' element */
  testsuites = doc.append_child("testsuites");
}

ReporterJUnit::Impl::~Impl() {
  /* -- save the XML report */
  doc.save_file(filename.c_str());
}

ReporterJUnit::ReporterJUnit(
    const std::string& file_) :
  pimpl(new Impl(file_)) {

}

ReporterJUnit::~ReporterJUnit() {
  odelete(pimpl);
}

void ReporterJUnit::enterTest(
    const Context& context_,
    const std::string& name_) {
  pimpl->testname = name_;
}

void ReporterJUnit::enterSuite(
    const Context& context_,
    const std::string& name_) {
  /* -- initialize state of the reporter for suite */
  pimpl->suite_start = context_.time_source->now();
  pimpl->case_count = 0;
  pimpl->case_failures = 0;
  pimpl->case_errors = 0;

  /* -- create suite's XML node */
  pimpl->suite = pimpl->testsuites.append_child("testsuite");

  /* -- name of the test */
  auto name_node_(pimpl->suite.append_attribute("name"));
  name_node_ = name_.c_str();

  /* -- timestamp of run */
  auto timestamp_(pimpl->suite.append_attribute("timestamp"));
  timestamp_ = formatTimestamp(pimpl->suite_start).c_str();
}

void ReporterJUnit::enterCase(
    const Context& context_,
    const std::string& name_) {
  /* -- initialize the case state */
  pimpl->case_start = context_.time_source->now();
  pimpl->case_first_failure = false;
  pimpl->case_first_error = false;
  ++pimpl->case_count;

  /* -- create the testcase XML node */
  pimpl->testcase = pimpl->suite.append_child("testcase");

  /* -- case name */
  auto case_name_(pimpl->testcase.append_attribute("name"));
  case_name_ = name_.c_str();
}

void ReporterJUnit::enterState(
    const Context& context_,
    const std::string& name_) {

}

void ReporterJUnit::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& message_,
    const std::string& file_,
    int lineno_) {
  if(!condition_) {
    /* -- update statistics */
    if(!pimpl->case_first_failure)
      ++pimpl->case_failures;
    else
      pimpl->case_first_failure = true;

    /* -- make the failure record */
    auto failure_(pimpl->testcase.append_child("failure"));
    auto message_attr_(failure_.append_attribute("message"));
    message_attr_ = message_.c_str();
    auto line_attr_(failure_.append_attribute("line"));
    line_attr_ = lineno_;
    auto file_attr_(failure_.append_attribute("file"));
    file_attr_ = file_.c_str();
  }
}

void ReporterJUnit::enterError(
    const Context& context_,
    const std::string& message_) {
  /* -- update statistics */
  if(!pimpl->case_first_error)
    ++pimpl->case_errors;
  else
    pimpl->case_first_error = true;

  /* -- make the failure record */
  auto error_(pimpl->testcase.append_child("error"));
  auto message_attr_(error_.append_attribute("message"));
  message_attr_ = message_.c_str();
}

void ReporterJUnit::reportAssertionMessage(
    const Context& context_,
    const std::string& message_) {

}

void ReporterJUnit::leaveAssert(
    const Context& context_) {

}

void ReporterJUnit::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {

}

void ReporterJUnit::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  auto end_time_(context_.time_source->now());

  /* -- time of run of the testcase (in seconds) */
  auto time_(pimpl->testcase.append_attribute("time"));
  time_ = formatDuration(pimpl->case_start, end_time_).c_str();
}

void ReporterJUnit::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  auto end_time_(context_.time_source->now());

  /* -- number of tests */
  auto tests_(pimpl->suite.append_attribute("tests"));
  tests_ = pimpl->case_count;

  /* -- number of failed tests */
  auto failures_(pimpl->suite.append_attribute("failures"));
  failures_ = pimpl->case_failures;

  /* -- number of error tests */
  auto errors_(pimpl->suite.append_attribute("errors"));
  errors_ = pimpl->case_errors;

  /* -- time of run of the suite (in seconds) */
  auto time_(pimpl->suite.append_attribute("time"));
  time_ = formatDuration(pimpl->suite_start, end_time_).c_str();
}

void ReporterJUnit::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {

}

} /* -- namespace OTest2 */

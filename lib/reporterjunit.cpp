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

#include <assert.h>
#include <chrono>
#include <iomanip>
#include <pugixml.hpp>
#include <sstream>
#include <string>
#include <ctime>
#include <time.h>
#include <vector>

#include <assertbufferstr.h>
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

struct ReporterJUnit::Impl : public AssertBufferListener {
    std::string filename;
    bool hide_location;

    /* -- XML structure */
    ::pugi::xml_document doc;

    /* -- name of the test (usually from the command line) */
    std::string testname;

    /* -- current active object */
    struct Record {
        ::pugi::xml_node node;
        TimeSource::time_point start;
        int case_count;
        bool first_failure;
        int case_failures;
        bool first_error;
        int case_errors;
    };
    std::vector<Record> node_stack;

    AssertBufferStrPtr assert_buffer;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& file_,
        bool hide_location_);
    virtual ~Impl();

    void cumulateStatistics(
        Record& target_,
        const Record& source_);
    void fillRunningTime(
        const Context& context_,
        Record& record_);
    void fillStatistics(
        const Context& context_,
        Record& record_);
    void fillTimestamp(
        const Context& context_,
        Record& record_);
    void fillName(
        const Context& context_,
        Record& record_,
        const std::string& name_);

    /* -- listener of the assertion buffer */
    virtual void assertionOpeningMessage(
        const Context& context_,
        const AssertBufferAssertData& data_,
        const std::string& message_) override;
    virtual void assertionAdditionalMessage(
        const Context& context_,
        const AssertBufferAssertData& data_,
        const std::string& message_) override;
    virtual void assertionClose(
        const Context& context_,
        const AssertBufferAssertData& data_) override;
    virtual void errorOpeningMessage(
        const Context& context_,
        const std::string& message_) override;
    virtual void errorAdditionalMessage(
        const Context& context_,
        const std::string& message_) override;
    virtual void errorClose(
        const Context& context_) override;
};

ReporterJUnit::Impl::Impl(
    const std::string& file_,
    bool hide_location_) :
  filename(file_),
  hide_location(hide_location_),
  testname(),
  node_stack(),
  assert_buffer(std::make_shared<AssertBufferStr>(this)) {

}

ReporterJUnit::Impl::~Impl() {

}

void ReporterJUnit::Impl::cumulateStatistics(
    Record& target_,
    const Record& source_) {
  target_.case_count += source_.case_count;
  target_.case_failures += source_.case_failures;
  target_.case_errors += source_.case_errors;
}

void ReporterJUnit::Impl::fillRunningTime(
    const Context& context_,
    Record& record_) {
  /* -- time of run of the suite (in seconds) */
  auto end_time_(context_.time_source->now());
  auto time_(record_.node.append_attribute("time"));
  time_ = formatDuration(record_.start, end_time_).c_str();
}

void ReporterJUnit::Impl::fillStatistics(
    const Context& context_,
    Record& record_) {
  /* -- number of tests */
  auto tests_(record_.node.append_attribute("tests"));
  tests_ = record_.case_count;

  /* -- number of failed tests */
  auto failures_(record_.node.append_attribute("failures"));
  failures_ = record_.case_failures;

  /* -- number of error tests */
  auto errors_(record_.node.append_attribute("errors"));
  errors_ = record_.case_errors;
}

void ReporterJUnit::Impl::fillTimestamp(
    const Context& context_,
    Record& record_) {
  /* -- timestamp of run */
  auto timestamp_(record_.node.append_attribute("timestamp"));
  timestamp_ = formatTimestamp(record_.start).c_str();
}

void ReporterJUnit::Impl::fillName(
    const Context& context_,
    Record& record_,
    const std::string& name_) {
  /* -- name of the test */
  auto name_node_(record_.node.append_attribute("name"));
  name_node_ = name_.c_str();
}

void ReporterJUnit::Impl::assertionOpeningMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  /* -- make the failure record */
  if(!data_.condition) {
    auto& top_(node_stack.back());
    auto failure_(top_.node.append_child("failure"));
    if(!hide_location) {
      auto line_attr_(failure_.append_attribute("line"));
      line_attr_ = data_.line;
      auto file_attr_(failure_.append_attribute("file"));
      file_attr_ = data_.file.c_str();
    }
    auto msg_(failure_.append_attribute("message"));
    msg_ = message_.c_str();
  }
}

void ReporterJUnit::Impl::assertionAdditionalMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  /* -- not supported by the JUnit format */
}

void ReporterJUnit::Impl::assertionClose(
    const Context& context_,
    const AssertBufferAssertData& data_) {
  /* -- nothing to do - the XML nodes are already created */
}

void ReporterJUnit::Impl::errorOpeningMessage(
    const Context& context_,
    const std::string& message_) {
  auto& top_(node_stack.back());
  auto error_(top_.node.append_child("error"));
  auto msg_(error_.append_attribute("message"));
  msg_ = message_.c_str();
}

void ReporterJUnit::Impl::errorAdditionalMessage(
    const Context& context_,
    const std::string& message_) {
  /* -- not supported by the JUnit format */
}

void ReporterJUnit::Impl::errorClose(
    const Context& context_) {
  /* -- nothing to do - the XML nodes are already created */
}

ReporterJUnit::ReporterJUnit(
    const std::string& file_,
    bool hide_location_) :
  pimpl(new Impl(file_, hide_location_)) {

}

ReporterJUnit::~ReporterJUnit() {
  odelete(pimpl);
}

void ReporterJUnit::enterTest(
    const Context& context_,
    const std::string& name_) {
  pimpl->testname = name_;

  /* -- create root node and root test suite (CircleCI ignores standalone
   *    test cases) */
  auto root_(pimpl->doc.append_child("testsuites"));
  pimpl->node_stack.push_back({
    root_.append_child("testsuite"),
    context_.time_source->now(),
    0,
    true,
    0,
    true,
    0,
  });
  pimpl->fillTimestamp(context_, pimpl->node_stack.back());
}

void ReporterJUnit::enterSuite(
    const Context& context_,
    const std::string& name_) {
  /* -- create new node */
  pimpl->node_stack.push_back({
    pimpl->node_stack.back().node.append_child("testsuite"),
    context_.time_source->now(),
    0,
    true,
    0,
    true,
    0,
  });
  pimpl->fillName(context_, pimpl->node_stack.back(), name_);
  pimpl->fillTimestamp(context_, pimpl->node_stack.back());
}

void ReporterJUnit::enterCase(
    const Context& context_,
    const std::string& name_) {
  /* -- create new node */
  pimpl->node_stack.push_back({
    pimpl->node_stack.back().node.append_child("testcase"),
    context_.time_source->now(),
    1,
    true,
    0,
    true,
    0,
  });
  pimpl->fillName(context_, pimpl->node_stack.back(), name_);
}

void ReporterJUnit::enterState(
    const Context& context_,
    const std::string& name_) {
  /* -- nothing to do - the states are not supported by the JUnit report */
}

AssertBufferPtr ReporterJUnit::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& file_,
    int lineno_) {
  if(!condition_) {
    /* -- update statistics */
    auto& top_(pimpl->node_stack.back());
    if(top_.first_failure) {
      top_.first_failure = false;
      ++top_.case_failures;
    }
  }

  pimpl->assert_buffer->openAssertion({condition_, file_, lineno_});
  return pimpl->assert_buffer;
}

AssertBufferPtr ReporterJUnit::enterError(
    const Context& context_) {
  /* -- update statistics */
  auto& top_(pimpl->node_stack.back());
  if(top_.first_error) {
    top_.first_error = false;
    ++top_.case_errors;
  }

  pimpl->assert_buffer->openError();
  return pimpl->assert_buffer;
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
  auto top_(pimpl->node_stack.back());

  /* -- fill test case attributes */
  pimpl->fillRunningTime(context_, top_);

  /* -- pop the object and cumulate statistics with the parent */
  pimpl->node_stack.pop_back();
  pimpl->cumulateStatistics(pimpl->node_stack.back(), top_);
}

void ReporterJUnit::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  auto top_(pimpl->node_stack.back());

  /* -- fill suite attributes */
  pimpl->fillRunningTime(context_, top_);
  pimpl->fillStatistics(context_, top_);

  /* -- pop the object and cumulate statistics with the parent */
  pimpl->node_stack.pop_back();
  pimpl->cumulateStatistics(pimpl->node_stack.back(), top_);
}

void ReporterJUnit::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  auto top_(pimpl->node_stack.back());

  /* -- fill suite attributes */
  pimpl->fillRunningTime(context_, top_);
  pimpl->fillStatistics(context_, top_);

  /* -- remove the root test suite */
  pimpl->node_stack.pop_back();

  /* -- save the XML report */
  pimpl->doc.save_file(pimpl->filename.c_str());
}

} /* -- namespace OTest2 */

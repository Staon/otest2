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

#include "reportermock.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <otest2/assertbean.h>
#include <otest2/assertbufferstr.h>
#include <otest2/context.h>
#include <otest2/objectpath.h>
#include <otest2/utils.h>

namespace OTest2 {

namespace Test {

namespace {

std::ostream& formatRecord(
    std::ostream& os_,
    const std::string& text_) {
  os_ << '"';
  for(char c_ : text_) {
    switch(c_) {
      case '"': os_ << "\\\""; break;
      case '\\': os_ << "\\\\"; break;
      case '\n': os_ << "\\n\\\n"; break;
      default: os_ << c_; break;
    }
  }
  os_ << '"';

  return os_;
}

} /* -- namespace */

struct ReporterMock::Impl : public AssertBufferListener {
  public:
    typedef std::vector<std::string> Records;
    Records records;
    std::ostringstream oss;
    bool report_paths;

    AssertBufferStrPtr assert_buffer;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        bool report_paths_);
    virtual ~Impl();

    void printResult(
        bool result_);
    void addRecord();
    void reportObjectPath(
        const Context& context_);

    /* -- assertion buffer listener */
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

ReporterMock::Impl::Impl(
    bool report_paths_) :
  report_paths(report_paths_),
  assert_buffer(std::make_shared<AssertBufferStr>(this)){

}

ReporterMock::Impl::~Impl() {

}

void ReporterMock::Impl::printResult(
    bool result_) {
  if(result_)
    oss << "passed";
  else
    oss << "failed";
}

void ReporterMock::Impl::addRecord() {
  records.push_back(oss.str());
  oss.str("");
}

void ReporterMock::Impl::reportObjectPath(
    const Context& context_) {
  if(report_paths) {
    oss << "path: " << context_.object_path->getCurrentPath();
    addRecord();
  }
}

void ReporterMock::Impl::assertionOpeningMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  /* -- regular assertion */
  oss << "assert<" << message_ << ">: ";
  printResult(data_.condition);
  addRecord();
  reportObjectPath(context_);
}

void ReporterMock::Impl::assertionAdditionalMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  oss << "message<" << message_ << ">";
  addRecord();
  reportObjectPath(context_);
}

void ReporterMock::Impl::assertionClose(
    const Context& context_,
    const AssertBufferAssertData& data_) {
  oss << "leaveAssert<>";
  addRecord();
  reportObjectPath(context_);
}

void ReporterMock::Impl::errorOpeningMessage(
    const Context& context_,
    const std::string& message_) {
  /* -- internal error */
  oss << "error<" << message_ << ">: ";
  printResult(false);
  addRecord();
  reportObjectPath(context_);
}

void ReporterMock::Impl::errorAdditionalMessage(
    const Context& context_,
    const std::string& message_) {
  oss << "errMessage<" << message_ << ">";
  addRecord();
  reportObjectPath(context_);
}

void ReporterMock::Impl::errorClose(
    const Context& context_) {
  oss << "leaveError<>";
  addRecord();
  reportObjectPath(context_);
}

ReporterMock::ReporterMock(
    bool report_paths_) :
  pimpl(new Impl(report_paths_)) {

}

ReporterMock::~ReporterMock() {
  odelete(pimpl);
}

void ReporterMock::reportDelay(
    int delay_ms_) {
  pimpl->oss << "delay<" << delay_ms_ << ">";
  pimpl->addRecord();
}

void ReporterMock::reportDebugMessage(
    const std::string& message_) {
  pimpl->oss << "debug<" << message_ << ">";
  pimpl->addRecord();
}

std::ostream& ReporterMock::dumpRecords(
    std::ostream& os_) const {
  os_ << "std::vector<const char*> data_{" << std::endl;
  for(const auto& record : pimpl->records) {
    os_ << "  ";
    formatRecord(os_, record);
    os_ << "," << std::endl;
  }
  os_ << "};" << std::endl;

  return os_;
}

AssertBean ReporterMock::checkRecords(
    const std::vector<const char*>& data_) const {
  /* -- check the size */
  if(pimpl->records.size() != data_.size())
    return AssertBean(false, "different data size");

  for(std::vector<const char*>::size_type i_(0); i_ < data_.size(); ++i_) {
    if(pimpl->records[i_] != data_[i_]) {
      std::ostringstream oss_;
      oss_ << "different item " << i_ << ": '" << pimpl->records[i_]
           << "' != '" << data_[i_] << "'";
      return AssertBean(false, oss_.str());
    }
  }

  return AssertBean(true, "OK");
}

void ReporterMock::enterTest(
    const Context& context_,
    const std::string& name_) {
  pimpl->oss << "enterTest<" << name_ << ">";
  pimpl->addRecord();
  pimpl->reportObjectPath(context_);
}

void ReporterMock::enterSuite(
    const Context& context_,
    const std::string& name_) {
  pimpl->oss << "enterSuite<" << name_ << ">";
  pimpl->addRecord();
  pimpl->reportObjectPath(context_);
}

void ReporterMock::enterCase(
    const Context& context_,
    const std::string& name_) {
  pimpl->oss << "enterCase<" << name_ << ">";
  pimpl->addRecord();
  pimpl->reportObjectPath(context_);
}

void ReporterMock::enterState(
    const Context& context_,
    const std::string& name_) {
  pimpl->oss << "enterState<" << name_ << ">";
  pimpl->addRecord();
  pimpl->reportObjectPath(context_);
}

AssertBufferPtr ReporterMock::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& file_,
    int lineno_) {
  pimpl->assert_buffer->openAssertion({condition_, file_, lineno_});
  return pimpl->assert_buffer;
}

AssertBufferPtr ReporterMock::enterError(
    const Context& context_) {
  pimpl->assert_buffer->openError();
  return pimpl->assert_buffer;
}

void ReporterMock::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->oss << "leaveState<" << name_ << ">: ";
  pimpl->printResult(result_);
  pimpl->addRecord();
  pimpl->reportObjectPath(context_);
}

void ReporterMock::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->oss << "leaveCase<" << name_ << ">: ";
  pimpl->printResult(result_);
  pimpl->addRecord();
  pimpl->reportObjectPath(context_);
}

void ReporterMock::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->oss << "leaveSuite<" << name_ << ">: ";
  pimpl->printResult(result_);
  pimpl->addRecord();
  pimpl->reportObjectPath(context_);
}

void ReporterMock::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->oss << "leaveTest<" << name_ << ">: ";
  pimpl->printResult(result_);
  pimpl->addRecord();
  pimpl->reportObjectPath(context_);
}

} /* namespace Test */

} /* namespace OTest2 */

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

struct ReporterMock::Impl {
  public:
    typedef std::vector<std::string> Records;
    Records records;
    std::ostringstream oss;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    Impl();
    ~Impl();

    void printResult(
        bool result_);
    void addRecord();
};

ReporterMock::Impl::Impl() {

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

ReporterMock::ReporterMock() :
  pimpl(new Impl) {

}

ReporterMock::~ReporterMock() {
  odelete(pimpl);
}

void ReporterMock::reportDelay(
    int delay_ms_) {
  pimpl->oss << "delay<" << delay_ms_ << ">";
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
}

void ReporterMock::enterSuite(
    const Context& context_,
    const std::string& name_) {
  pimpl->oss << "enterSuite<" << name_ << ">";
  pimpl->addRecord();
}

void ReporterMock::enterCase(
    const Context& context_,
    const std::string& name_) {
  pimpl->oss << "enterCase<" << name_ << ">";
  pimpl->addRecord();
}

void ReporterMock::enterState(
    const Context& context_,
    const std::string& name_) {
  pimpl->oss << "enterState<" << name_ << ">";
  pimpl->addRecord();
}

void ReporterMock::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& message_,
    const std::string& file_,
    int lineno_) {
  pimpl->oss << "assert<" << message_ << ">: ";
  pimpl->printResult(condition_);
  pimpl->addRecord();
}

void ReporterMock::enterError(
    const Context& context_,
    const std::string& message_) {
  pimpl->oss << "error<" << message_ << ">: ";
  pimpl->printResult(false);
  pimpl->addRecord();
}

void ReporterMock::reportAssertionMessage(
    const Context& context_,
    const std::string& message_) {
  pimpl->oss << "message<" << message_ << ">: ";
  pimpl->printResult(false);
  pimpl->addRecord();
}

void ReporterMock::leaveAssert(
    const Context& context_) {
  pimpl->oss << "leaveAssert<>";
  pimpl->addRecord();
}

void ReporterMock::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->oss << "leaveState<" << name_ << ">: ";
  pimpl->printResult(result_);
  pimpl->addRecord();
}

void ReporterMock::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->oss << "leaveCase<" << name_ << ">: ";
  pimpl->printResult(result_);
  pimpl->addRecord();
}

void ReporterMock::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->oss << "leaveSuite<" << name_ << ">: ";
  pimpl->printResult(result_);
  pimpl->addRecord();
}

void ReporterMock::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->oss << "leaveTest<" << name_ << ">: ";
  pimpl->printResult(result_);
  pimpl->addRecord();
}

} /* namespace Test */

} /* namespace OTest2 */

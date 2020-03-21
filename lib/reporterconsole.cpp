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
#include <reporterconsole.h>

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <context.h>
#include <objectpath.h>
#include <reporterstatistics.h>
#include <terminaldriver.h>
#include <utils.h>

namespace OTest2 {

namespace {

enum : int {
  WIDTH = 80,
};

void printHR(
    std::ostream& os_,
    char rule_char_,
    const std::string& label_) {
  /* -- label width */
  int label_width_(0);
  if(!label_.empty()) {
    label_width_ = label_.size() + 2;
  }

  /* -- compute length of the left and righ part of the rule */
  int left_width_((WIDTH - 2 - label_width_) / 2);
  int right_width_(WIDTH - 2 - label_width_ - left_width_);

  /* -- print left rule */
  os_ << ' ';
  for(int i_(0); i_ < left_width_; ++i_)
    os_ << rule_char_;

  /* -- print label */
  if(!label_.empty()) {
    os_ << ' ' << label_ << ' ';
  }

  /* -- print right rule */
  for(int i_(0); i_ < right_width_; ++i_)
    os_ << rule_char_;

  os_ << std::endl;
}

void printResultLine(
    std::ostream& os_,
    TerminalDriver& driver_,
    const std::string& label_,
    bool result_) {
  int label_width_(label_.size());
  int free_space_width_(WIDTH - 4 - label_width_ - 8);

  os_ << "  " << label_;
  for(int i_(0); i_ < free_space_width_; ++i_)
    os_ << ' ';
  os_ << '[';

  if(result_) {
    driver_.setFGColor(TerminalDriver::GREEN);
    os_ << "Passed";
    driver_.cleanAttributes();
  }
  else {
    driver_.setFGColor(TerminalDriver::RED);
    os_ << "Failed";
    driver_.cleanAttributes();
  }
  os_ << ']' << std::endl;
}

void printTotalResultLine(
    std::ostream& os_,
    const std::string& label_,
    const std::string& ok_,
    const std::string& failed_,
    const std::string& total_) {
  int label_width_(label_.size());
  int ok_width_(ok_.size());
  int failed_width_(failed_.size());

  enum {
    OK_COLUMN = 20,
    FAILED_COLUMN = 40,
    TOTAL_COLUMN = 60,
  };

  os_ << "  " << label_;
  int i_(2 + label_width_);
  for(; i_ < OK_COLUMN; ++i_)
    os_ << ' ';
  os_ << ok_;
  for(i_ += ok_width_; i_ < FAILED_COLUMN; ++i_)
    os_ << ' ';
  os_ << failed_;
  for(i_ += failed_width_; i_ < TOTAL_COLUMN; ++i_)
    os_ << ' ';
  os_ << total_ << std::endl;
}

void printTotalResult(
    std::ostream& os_,
    const std::string& label_,
    int ok_,
    int failed_,
    int total_) {
  std::ostringstream ok_str_;
  ok_str_ << std::setw(8) << ok_;
  std::ostringstream failed_str_;
  failed_str_ << std::setw(8) << failed_;
  std::ostringstream total_str_;
  total_str_ << std::setw(8) << total_;
  printTotalResultLine(
      os_, label_, ok_str_.str(), failed_str_.str(), total_str_.str());
}

} /* -- namespace */

struct ReporterConsole::Impl {
  public:
    ReporterConsole* owner;
    std::ostream* os;
    TerminalDriver term_driver;
    ReporterStatistics statistics;

    /* -- assertion printing */
    bool verbose;
    bool last_condition;

  public:
    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        ReporterConsole* owner_,
        std::ostream* os_,
        bool verbose_);
    ~Impl();
};

ReporterConsole::Impl::Impl(
    ReporterConsole* owner_,
    std::ostream* os_,
    bool verbose_) :
  owner(owner_),
  os(os_),
  term_driver(os_),
  verbose(verbose_),
  last_condition(true) {
  assert(os != nullptr);

}

ReporterConsole::Impl::~Impl() {

}

ReporterConsole::ReporterConsole(
    std::ostream* os_,
    bool verbose_) :
  pimpl(new Impl(this, os_, verbose_)) {

}

ReporterConsole::~ReporterConsole() {
  odelete(pimpl);
}

void ReporterConsole::enterTest(
    const Context& context_,
    const std::string& name_) {

}

void ReporterConsole::enterSuite(
    const Context& context_,
    const std::string& name_) {
  printHR(*pimpl->os, '=', name_);
}

void ReporterConsole::enterCase(
    const Context& context_,
    const std::string& name_) {

}

void ReporterConsole::enterState(
    const Context& context_,
    const std::string& name_) {

}

void ReporterConsole::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& message_,
    const std::string& file_,
    int lineno_) {
  /* -- adjust the statistics */
  pimpl->statistics.reportAssertion(condition_);

  /* -- store the condition for printing of additional messages */
  pimpl->last_condition = condition_;

  /* -- print the assertion status */
  if(pimpl->verbose || !condition_) {
    *pimpl->os
        << '[' << file_ << ':' << lineno_ << "] "
        << context_.object_path->getCurrentPath()  << ": " << message_
        << std::endl;
  }
}

void ReporterConsole::enterError(
    const Context& context_,
    const std::string& message_) {
  /* -- adjust the statistics */
  pimpl->statistics.reportAssertion(false);

  /* -- store the condition for printing of additional messages */
  pimpl->last_condition = false;

  /* -- print the error */
  *pimpl->os
      << "error " << context_.object_path->getCurrentPath() << ": " << message_
      << std::endl;
}

void ReporterConsole::reportAssertionMessage(
    const Context& context_,
    const std::string& message_) {
  if(pimpl->verbose || !pimpl->last_condition) {
    std::istringstream iss_(message_);
    std::string line_;
    while(std::getline(iss_, line_))
      *pimpl->os << "  " << line_ << std::endl;
  }
}

void ReporterConsole::leaveAssert(
    const Context& context_) {
  /* -- nothing to do */
}

void ReporterConsole::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {

}

void ReporterConsole::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->statistics.reportCase(result_);

  printResultLine(*pimpl->os, pimpl->term_driver, name_, result_);
}

void ReporterConsole::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->statistics.reportSuite(result_);

  printHR(*pimpl->os, '-', "");
  printResultLine(*pimpl->os, pimpl->term_driver, "Suite total", result_);
}

void ReporterConsole::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  printHR(*pimpl->os, '=', "Test results");
  printTotalResultLine(
      *pimpl->os,
      "",
      "  Passed",
      "  Failed",
      "   Total");
  printTotalResult(
      *pimpl->os,
      "Suites",
      pimpl->statistics.getSuitesOK(),
      pimpl->statistics.getSuitesFailed(),
      pimpl->statistics.getSuites());
  printTotalResult(
      *pimpl->os,
      "Cases",
      pimpl->statistics.getCasesOK(),
      pimpl->statistics.getCasesFailed(),
      pimpl->statistics.getCases());
  printTotalResult(
      *pimpl->os,
      "Checks",
      pimpl->statistics.getAssertionsOK(),
      pimpl->statistics.getAssertionsFailed(),
      pimpl->statistics.getAssertions());
  printResultLine(*pimpl->os, pimpl->term_driver, "Test total", result_);
  printHR(*pimpl->os, '=', "");
}

} /* namespace OTest2 */

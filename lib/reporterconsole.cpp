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
#include <utility>

#include <assertbufferstr.h>
#include <context.h>
#include <objectpath.h>
#include <reporterstatistics.h>
#include <terminaldriver.h>
#include <utils.h>

namespace OTest2 {

namespace {

enum : int {
  WIDTH = 80,
  INDENT_SPACE = 2,
};

void printIndent(
    std::ostream& os_,
    int indent_) {
  for(int i_(0); i_ < indent_; ++i_)
    for(int j_(0); j_ < INDENT_SPACE; ++j_)
      os_ << ' ';
}

void printHR(
    std::ostream& os_,
    char rule_char_,
    const std::string& label_,
    int indent_) {
  /* -- label width */
  int label_width_(0);
  if(!label_.empty()) {
    label_width_ = static_cast<int>(label_.size()) + 2;
  }

  /* -- compute length of the left and righ part of the rule */
  const int width_(WIDTH - 2 - INDENT_SPACE * indent_);
  int left_width_((width_ - label_width_) / 2);
  int right_width_(width_ - label_width_ - left_width_);

  /* -- print indentation */
  printIndent(os_, indent_);

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
    bool result_,
    int indent_) {
  int label_width_(static_cast<int>(label_.size()));
  int free_space_width_(WIDTH - 4 - INDENT_SPACE * indent_ - label_width_ - 8);

  printIndent(os_, indent_);

  os_ << "  " << label_;
  for(int i_(0); i_ < free_space_width_; ++i_)
    os_ << ' ';
  os_ << '[';

  if(result_) {
    driver_.setForeground(*os_.rdbuf(), Color::GREEN);
    os_ << "Passed";
  }
  else {
    driver_.setForeground(*os_.rdbuf(), Color::RED);
    os_ << "Failed";
  }
  driver_.cleanAttributes(*os_.rdbuf());
  os_ << ']' << std::endl;
}

void printTotalResultLine(
    std::ostream& os_,
    const std::string& label_,
    const std::string& ok_,
    const std::string& failed_,
    const std::string& total_) {
  int label_width_(static_cast<int>(label_.size()));
  int ok_width_(static_cast<int>(ok_.size()));
  int failed_width_(static_cast<int>(failed_.size()));

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

void printTotalErrors(
    std::ostream& os_,
    const std::string& label_,
    int errors_) {
  std::ostringstream errors_str_;
  errors_str_ << std::setw(8) << errors_;
  printTotalResultLine(os_, label_, "        ", "        ", errors_str_.str());
}

} /* -- namespace */

struct ReporterConsole::Impl : public AssertBufferListener {
  private:
    class AssertBuffer : public AssertBufferStr {
      private:
        Impl* pimpl;

      public:
        /* -- avoid copying */
        AssertBuffer(
            const AssertBuffer&) = delete;
        AssertBuffer& operator = (
            const AssertBuffer&) = delete;

        explicit AssertBuffer(
            Impl* pimpl_);
        virtual ~AssertBuffer();

        /* -- setting of text attributes */
        virtual void setForeground(
            Color color_) override;
        virtual void setBackground(
            Color color_) override;
        virtual void setTextStyle(
            Style style_) override;
        virtual void resetAttributes() override;
    };

    static int selectHandle(
        std::ostream* os_);

  public:
    ReporterConsole* owner;
    std::ostream* os;
    TerminalDriver term_driver;
    ReporterStatistics statistics;

    /* -- reporter state */
    int level;

    /* -- assertion printing */
    bool verbose;
    bool hide_location;
    int indent;
    std::pair<int, char> stacked_hr;

    /* -- buffer for assertion messages */
    AssertBufferStrPtr assert_buffer;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        ReporterConsole* owner_,
        std::ostream* os_,
        bool verbose_,
        bool hide_location_);
    virtual ~Impl();

    void printStackedHR();
    void resetStackedHR();

    void printAdditionalMessage(
        bool condition_,
        const std::string& message_);

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

ReporterConsole::Impl::AssertBuffer::AssertBuffer(
    Impl* pimpl_) :
  AssertBufferStr(pimpl_),
  pimpl(pimpl_) {

}

ReporterConsole::Impl::AssertBuffer::~AssertBuffer() = default;

void ReporterConsole::Impl::AssertBuffer::setForeground(
    Color color_) {
  pimpl->term_driver.setForeground(*this, color_);
}

void ReporterConsole::Impl::AssertBuffer::setBackground(
    Color color_) {
  pimpl->term_driver.setBackground(*this, color_);
}

void ReporterConsole::Impl::AssertBuffer::setTextStyle(
    Style style_) {
  pimpl->term_driver.setTextStyle(*this, style_);
}

void ReporterConsole::Impl::AssertBuffer::resetAttributes() {
  pimpl->term_driver.cleanAttributes(*this);
}

ReporterConsole::Impl::Impl(
    ReporterConsole* owner_,
    std::ostream* os_,
    bool verbose_,
    bool hide_location_) :
  owner(owner_),
  os(os_),
  term_driver(selectHandle(os_)),
  level(0),
  verbose(verbose_),
  hide_location(hide_location_),
  indent(-1),
  stacked_hr(0, '='),
  assert_buffer(std::make_shared<AssertBuffer>(this)) {
  assert(os != nullptr);

}

ReporterConsole::Impl::~Impl() = default;

int ReporterConsole::Impl::selectHandle(
    std::ostream* os_) {
  if(os_ == &std::cout)
    return 1;
  else if(os_ == &std::cerr)
    return 2;
  else
    return -1;
}

void ReporterConsole::Impl::printStackedHR() {
  if(stacked_hr.first >= 0)
    printHR(*os, stacked_hr.second, "", stacked_hr.first);
  stacked_hr = {-1, ' '};
}

void ReporterConsole::Impl::resetStackedHR() {
  stacked_hr = {-1, ' '};
}

void ReporterConsole::Impl::printAdditionalMessage(
    bool condition_,
    const std::string& message_) {
  if(verbose || !condition_) {
    std::istringstream iss_(message_);
    std::string line_;
    while(std::getline(iss_, line_))
      *os << "    " << line_ << std::endl;
  }
}

void ReporterConsole::Impl::assertionOpeningMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  printStackedHR();

  /* -- print the assertion status */
  if(verbose || !data_.condition) {
    /* -- make the opening message more visible */
    term_driver.setTextStyle(*os->rdbuf(), Style::BOLD);

    /* -- print the message */
    *os << '[';
    if(!hide_location)
      *os << data_.file << ':' << data_.line;
    else
      *os << "...";
    *os << "] "
        << context_.object_path->getCurrentPath()  << ": " << message_
        << std::endl;

    /* -- clean text attributes */
    term_driver.cleanAttributes(*os->rdbuf());
  }
}

void ReporterConsole::Impl::assertionAdditionalMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  printAdditionalMessage(data_.condition, message_);
}

void ReporterConsole::Impl::assertionClose(
    const Context& context_,
    const AssertBufferAssertData& data_) {
  /* -- nothing to do */
}

void ReporterConsole::Impl::errorOpeningMessage(
    const Context& context_,
    const std::string& message_) {
  printStackedHR();

  /* -- internal error */
  *os << "error in " << context_.object_path->getCurrentPath() << ": " << message_
      << std::endl;
}

void ReporterConsole::Impl::errorAdditionalMessage(
    const Context& context_,
    const std::string& message_) {
  printAdditionalMessage(false, message_);
}

void ReporterConsole::Impl::errorClose(
    const Context& context_) {
  /* -- nothing to do */
}

ReporterConsole::ReporterConsole(
    std::ostream* os_,
    bool verbose_,
    bool hide_location_) :
  pimpl(new Impl(this, os_, verbose_, hide_location_)) {

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
  /* -- print the separator */
  ++pimpl->indent;
  pimpl->resetStackedHR();
  printHR(*pimpl->os, '=', name_, pimpl->indent);

  /* -- increase level of nesting */
  ++pimpl->level;
}

void ReporterConsole::enterCase(
    const Context& context_,
    const std::string& name_) {
  /* -- correct indentation of a standalone test case */
  if(pimpl->level == 0)
    pimpl->indent = 0;

  /* -- increase level of nesting */
  ++pimpl->level;
}

void ReporterConsole::enterState(
    const Context& context_,
    const std::string& name_) {

}

AssertBufferPtr ReporterConsole::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& file_,
    int lineno_) {
  /* -- adjust the statistics */
  pimpl->statistics.reportAssertion(condition_);

  /* -- open the assertion buffer */
  pimpl->assert_buffer->openAssertion({condition_, file_, lineno_});
  return pimpl->assert_buffer;
}

AssertBufferPtr ReporterConsole::enterError(
    const Context& context_) {
  /* -- adjust the statistics */
  pimpl->statistics.reportError();

  /* -- open the assertion buffer */
  pimpl->assert_buffer->openError();
  return pimpl->assert_buffer;
}

void ReporterConsole::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  /* -- nothing to do */
}

void ReporterConsole::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->statistics.reportCase(result_);

  /* -- print result of the test case */
  pimpl->printStackedHR();
  printResultLine(*pimpl->os, pimpl->term_driver, name_, result_, pimpl->indent);

  /* -- decrease level of nesting and indentation */
  --pimpl->level;
  if(pimpl->level == 0)
    --pimpl->indent;
}

void ReporterConsole::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->statistics.reportSuite(result_);

  /* -- print result of the suite */
  pimpl->resetStackedHR();
  printHR(*pimpl->os, '-', "", pimpl->indent);
  printResultLine(*pimpl->os, pimpl->term_driver, "Suite total", result_, pimpl->indent);

  /* -- stack the suite ending separator */
  pimpl->stacked_hr = {pimpl->indent, '='};

  /* -- decrease level of nesting and indentation */
  --pimpl->level;
  --pimpl->indent;
}

void ReporterConsole::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  pimpl->resetStackedHR();
  printHR(*pimpl->os, '=', "Test results", 0);
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
  printTotalErrors(*pimpl->os, "Errors", pimpl->statistics.getErrors());
  printResultLine(*pimpl->os, pimpl->term_driver, "Test total", result_, 0);
  printHR(*pimpl->os, '=', "", 0);
}

} /* namespace OTest2 */

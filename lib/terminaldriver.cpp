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

#include <terminaldriver.h>

#include <assert.h>
#include <curses.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <streambuf>
#include <string>
#include <term.h>
#include <unistd.h>

#include <utils.h>

namespace OTest2 {

namespace {

class Driver {
  public:
    /* -- avoid copying */
    Driver(
        const Driver&) = delete;
    Driver& operator =(
        const Driver&) = delete;

    Driver();
    virtual ~Driver();

    virtual void setForeground(
        std::streambuf& buffer_,
        Color color_) = 0;
    virtual void setBackground(
        std::streambuf& buffer_,
        Color color_) = 0;
    virtual void setTextStyle(
        std::streambuf& buffer_,
        Style style_) = 0;
    virtual void cleanAttributes(
        std::streambuf& buffer_) = 0;

  protected:
    static void printSequence(
        std::streambuf& buffer_,
        const char* sequence_);
};

Driver::Driver() = default;
Driver::~Driver() = default;

void Driver::printSequence(
    std::streambuf& buffer_,
    const char* sequence_) {
  while(*sequence_ != 0) {
    buffer_.sputc(*sequence_);
    ++sequence_;
  }
}

class DriverNull : public Driver {
  public:
    /* -- avoid copying */
    DriverNull(
        const DriverNull&) = delete;
    DriverNull& operator =(
        const DriverNull&) = delete;

    DriverNull();
    virtual ~DriverNull();

    virtual void setForeground(
        std::streambuf& buffer_,
        Color color_) override;
    virtual void setBackground(
        std::streambuf& buffer_,
        Color color_) override;
    virtual void setTextStyle(
        std::streambuf& buffer_,
        Style style_) override;
    virtual void cleanAttributes(
        std::streambuf& buffer_) override;
};

DriverNull::DriverNull() = default;
DriverNull::~DriverNull() = default;

void DriverNull::setForeground(
    std::streambuf& buffer_,
    Color color_) {
  /* -- nothing to do */
}

void DriverNull::setBackground(
    std::streambuf& buffer_,
    Color color_) {
  /* -- nothing to do */
}

void DriverNull::setTextStyle(
    std::streambuf& buffer_,
    Style style_) {
  /* -- nothing to do */
}

void DriverNull::cleanAttributes(
    std::streambuf& buffer_) {
  /* -- nothing to do */
}

class DriverTerminfo : public Driver {
  private:
    const std::string sgr0;
    const std::string setaf;
    const std::string setf;
    const std::string setab;
    const std::string setb;
    const std::string bold;
    const std::string dim;

    static std::string initSequence(
        const char* seq_id_);

    static int convertAnsiColor(
        Color color_);
    static int convertLegacyColor(
        Color color_);

  public:
    /* -- avoid copying */
    DriverTerminfo(
        const DriverTerminfo&) = delete;
    DriverTerminfo& operator =(
        const DriverTerminfo&) = delete;

    explicit DriverTerminfo();
    virtual ~DriverTerminfo();

    virtual void setForeground(
        std::streambuf& buffer_,
        Color color_) override;
    virtual void setBackground(
        std::streambuf& buffer_,
        Color color_) override;
    virtual void setTextStyle(
        std::streambuf& buffer_,
        Style style_) override;
    virtual void cleanAttributes(
        std::streambuf& buffer_) override;
};

std::string DriverTerminfo::initSequence(
    const char* seq_id_) {
  const char* sequence_(tigetstr(seq_id_));
  if(sequence_ != nullptr)
    return std::string(sequence_);
  else
    return std::string();
}

int DriverTerminfo::convertAnsiColor(
    Color color_) {
  switch(color_) {
    case Color::BLACK:
      return 0;
    case Color::RED:
      return 1;
    case Color::GREEN:
      return 2;
    case Color::YELLOW:
      return 3;
    case Color::BLUE:
      return 4;
    case Color::MAGENTA:
      return 5;
    case Color::CYAN:
      return 6;
    case Color::WHITE:
      return 7;
    default:
      assert(false);
      return 7;
  }
}

int DriverTerminfo::convertLegacyColor(
    Color color_) {
  switch(color_) {
    case Color::BLACK:
      return 0;
    case Color::RED:
      return 4;
    case Color::GREEN:
      return 2;
    case Color::YELLOW:
      return 6;
    case Color::BLUE:
      return 1;
    case Color::MAGENTA:
      return 5;
    case Color::CYAN:
      return 3;
    case Color::WHITE:
      return 7;
    default:
      assert(false);
      return 7;
  }
}

DriverTerminfo::DriverTerminfo() :
  sgr0(initSequence("sgr0")),
  setaf(initSequence("setaf")),
  setf(initSequence("setf")),
  setab(initSequence("setab")),
  setb(initSequence("setb")),
  bold(initSequence("bold")),
  dim(initSequence("dim")) {

}

DriverTerminfo::~DriverTerminfo() = default;

void DriverTerminfo::setForeground(
    std::streambuf& buffer_,
    Color color_) {
  /* -- don't set attributes if they cannot be reset */
  if(sgr0.empty())
    return;

  /* -- ANSI colors */
  if(!setaf.empty()) {
    printSequence(buffer_, tparm(setaf.c_str(), convertAnsiColor(color_)));
    return;
  }

  /* -- legacy colors */
  if(!setf.empty()) {
    printSequence(buffer_, tparm(setf.c_str(), convertLegacyColor(color_)));
    return;
  }
}

void DriverTerminfo::setBackground(
    std::streambuf& buffer_,
    Color color_) {
  /* -- don't set attributes if they cannot be reset */
  if(sgr0.empty())
    return;

  /* -- ANSI colors */
  if(!setab.empty()) {
    printSequence(buffer_, tparm(setab.c_str(), convertAnsiColor(color_)));
    return;
  }

  /* -- legacy colors */
  if(!setb.empty()) {
    printSequence(buffer_, tparm(setb.c_str(), convertLegacyColor(color_)));
    return;
  }
}

void DriverTerminfo::setTextStyle(
    std::streambuf& buffer_,
    Style style_) {
  /* -- don't set attributes if they cannot be reset */
  if(sgr0.empty())
    return;

  switch(style_) {
    case Style::BOLD:
      if(!bold.empty())
        printSequence(buffer_, tparm(bold.c_str()));
      break;

    case Style::DIM:
      if(!dim.empty())
        printSequence(buffer_, tparm(dim.c_str()));
      break;

    default:
      assert(false);
      break;
  }
}

void DriverTerminfo::cleanAttributes(
    std::streambuf& buffer_) {
  if(!sgr0.empty()) {
    printSequence(buffer_, tparm(sgr0.c_str()));
  }
}

} /* -- namespace */

struct TerminalDriver::Impl {
  public:
    std::unique_ptr<Driver> driver;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        int handle_);
    ~Impl();
};

TerminalDriver::Impl::Impl(
    int handle_) {
  /* -- initialize the terminfo */
  if(handle_ >= 0 && isatty(handle_)) {
    int err_;
    int info_(setupterm(nullptr, handle_, &err_));
    if(info_ == OK && err_ == 1) {
      driver = ::OTest2::make_unique<DriverTerminfo>();
      return;
    }
  }

  /* -- In all other cases set the null driver up. */
  driver = ::OTest2::make_unique<DriverNull>();
}

TerminalDriver::Impl::~Impl() {

}

TerminalDriver::TerminalDriver(
    int handle_) :
  pimpl(new Impl(handle_)) {

}

TerminalDriver::~TerminalDriver() {
  odelete(pimpl);
}

void TerminalDriver::setForeground(
    std::streambuf& buffer_,
    Color color_) {
  pimpl->driver->setForeground(buffer_, color_);
}

void TerminalDriver::setBackground(
    std::streambuf& buffer_,
    Color color_) {
  pimpl->driver->setBackground(buffer_, color_);
}

void TerminalDriver::setTextStyle(
    std::streambuf& buffer_,
    Style style_) {
  pimpl->driver->setTextStyle(buffer_, style_);
}

void TerminalDriver::cleanAttributes(
    std::streambuf& buffer_) {
  pimpl->driver->cleanAttributes(buffer_);
}

} /* namespace OTest2 */

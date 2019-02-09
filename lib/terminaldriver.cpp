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

    virtual void setFGColor(
        std::ostream* os_,
        TerminalDriver::Color color_) = 0;
    virtual void cleanAttributes(
        std::ostream* os_) = 0;
};

Driver::Driver() {

}

Driver::~Driver() {

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

    virtual void setFGColor(
        std::ostream* os_,
        TerminalDriver::Color color_);
    virtual void cleanAttributes(
        std::ostream* os_);
};

DriverNull::DriverNull() {

}

DriverNull::~DriverNull() {

}

void DriverNull::setFGColor(
    std::ostream* os_,
    TerminalDriver::Color color_) {

}

void DriverNull::cleanAttributes(
    std::ostream* os_) {

}

class DriverTerminfo : public Driver {
  private:
    const std::string sgr0;
    const std::string setaf;
    const std::string setf;

  public:
    /* -- avoid copying */
    DriverTerminfo(
        const DriverTerminfo&) = delete;
    DriverTerminfo& operator =(
        const DriverTerminfo&) = delete;

    explicit DriverTerminfo(
        const std::string& sgr0_,
        const std::string& setaf_,
        const std::string& setf_);
    virtual ~DriverTerminfo();

    virtual void setFGColor(
        std::ostream* os_,
        TerminalDriver::Color color_);
    virtual void cleanAttributes(
        std::ostream* os_);
};

DriverTerminfo::DriverTerminfo(
    const std::string& sgr0_,
    const std::string& setaf_,
    const std::string& setf_) :
  sgr0(sgr0_),
  setaf(setaf_),
  setf(setf_) {

}

DriverTerminfo::~DriverTerminfo() {

}

void DriverTerminfo::setFGColor(
    std::ostream* os_,
    TerminalDriver::Color color_) {
  if(!setaf.empty()) {
    /* -- ANSI colors */
    int converted_(0);
    switch(color_) {
      case TerminalDriver::RED:
        converted_ = 1;
        break;
      case TerminalDriver::GREEN:
        converted_ = 2;
        break;
      default:
        assert(0);
    }
    *os_ << tparm(setaf.c_str(), converted_);
  }
  else {
    /* -- old color system */
    int converted_(0);
    switch(color_) {
      case TerminalDriver::RED:
        converted_ = 4;
        break;
      case TerminalDriver::GREEN:
        converted_ = 2;
        break;
      default:
        assert(0);
    }
    *os_ << tparm(setf.c_str(), converted_);
  }
}

void DriverTerminfo::cleanAttributes(
    std::ostream* os_) {
  *os_ << tparm(sgr0.c_str());
}

} /* -- namespace */

struct TerminalDriver::Impl {
  public:
    std::ostream* os;
    std::unique_ptr<Driver> driver;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        std::ostream* os_);
    ~Impl();
};

TerminalDriver::Impl::Impl(
    std::ostream* os_) :
  os(os_) {
  assert(os != nullptr);

  /* -- Decide the file handle. Now only standard output and standard
   *    error output are considered being terminals. */
  int handle_(-1);
  if(os == &std::cout)
    handle_ = 1;
  else if(os == &std::cerr)
    handle_ = 2;

  /* -- initialize the terminfo */
  if(handle_ >= 0 && isatty(handle_)) {
    int err_;
    int info_(setupterm(nullptr, handle_, &err_));
    if(info_ == OK && err_ == 1) {
      const char* setaf_(tigetstr("setaf"));
      const char* setf_(tigetstr("setf"));
      const char* sgr0_(tigetstr("sgr0"));
      if(sgr0_ > reinterpret_cast<char*>(0)
          && (setaf_ > reinterpret_cast<char*>(0)
              || setf_ > reinterpret_cast<char*>(0))) {
        /* -- the capabilities are correctly read, create the driver */
        driver.reset(new DriverTerminfo(
            sgr0_,
            (setaf_ > reinterpret_cast<char*>(0))?setaf_:"",
            (setf_ > reinterpret_cast<char*>(0))?setf_:""));
        return;
      }
    }
  }

  /* -- In all other cases set the null driver up. */
  driver.reset(new DriverNull);
}

TerminalDriver::Impl::~Impl() {

}

TerminalDriver::TerminalDriver(
    std::ostream* os_) :
  pimpl(new Impl(os_)) {

}

TerminalDriver::~TerminalDriver() {
  odelete(pimpl);
}

void TerminalDriver::setFGColor(
    TerminalDriver::Color color_) {
  pimpl->driver->setFGColor(pimpl->os, color_);
}

void TerminalDriver::cleanAttributes() {
  pimpl->driver->cleanAttributes(pimpl->os);
}

} /* namespace OTest2 */

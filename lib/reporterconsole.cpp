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

#include <utils.h>

namespace OTest2 {

struct ReporterConsole::Impl {
  public:
    ReporterConsole* owner;
    std::ostream* out;

  public:
    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        ReporterConsole* owner_,
        std::ostream* out_);
    ~Impl();
};

ReporterConsole::Impl::Impl(
    ReporterConsole* owner_,
    std::ostream* out_) :
  owner(owner_),
  out(out_) {
  assert(out != nullptr);

}

ReporterConsole::Impl::~Impl() {

}

ReporterConsole::ReporterConsole(
    std::ostream* out_) :
  pimpl(new Impl(this, out_)) {

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

}

void ReporterConsole::enterCase(
    const Context& context_,
    const std::string& name_) {

}

void ReporterConsole::enterState(
    const Context& context_,
    const std::string& name_) {

}

void ReporterConsole::reportAssert(
    const Context& context_,
    bool condition_,
    const std::string& message_,
    const std::string& file_,
    int lineno_) {

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

}

void ReporterConsole::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {

}

void ReporterConsole::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {

}

} /* namespace OTest2 */

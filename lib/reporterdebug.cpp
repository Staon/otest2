/*
 * Copyright (C) 2018 Ondrej Starek
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

#include <reporterdebug.h>

#include <assert.h>
#include <iostream>

namespace OTest2 {

ReporterDebug::ReporterDebug(
    std::ostream* os_) :
  os(os_) {
  assert(os != nullptr);

}


ReporterDebug::~ReporterDebug() {

}

void ReporterDebug::enterTest(
    const Context& context_,
    const std::string& name_) {
  *os << "enterTest(" << name_ << ")" << std::endl;
}

void ReporterDebug::enterSuite(
    const Context& context_,
    const std::string& name_) {
  *os << "enterSuite(" << name_ << ")" << std::endl;
}

void ReporterDebug::enterCase(
    const Context& context_,
    const std::string& name_) {
  *os << "enterCase(" << name_ << ")" << std::endl;
}

void ReporterDebug::enterState(
    const Context& context_,
    const std::string& name_) {
  *os << "enterState(" << name_ << ")" << std::endl;
}

void ReporterDebug::reportAssert(
    const Context& context_,
    bool condition_,
    const std::string& message_,
    const std::string& file_,
    int lineno_) {
  *os << "assert at " << file_ << ":" << lineno_ << ": " << message_
      << " " << condition_ << std::endl;
}

void ReporterDebug::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  *os << "leaveState(" << name_ << ", " << result_ << ")" << std::endl;
}

void ReporterDebug::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  *os << "leaveCase(" << name_ << ", " << result_ << ")" << std::endl;
}

void ReporterDebug::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  *os << "leaveSuite(" << name_ << ", " << result_ << ")" << std::endl;
}

void ReporterDebug::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  *os << "leaveTest(" << name_ << ", " << result_ << ")" << std::endl;
}

} /* namespace OTest2 */
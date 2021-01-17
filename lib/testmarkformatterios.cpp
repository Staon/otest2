/*
 * Copyright (C) 2021 Ondrej Starek
 *
 * This file is part of OTest2
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

#include <testmarkformatterios.h>

#include <assert.h>
#include <iostream>
#include <sstream>

#include <testmark.h>

namespace OTest2 {

TestMarkFormatterIOS::TestMarkFormatterIOS(
    std::ostream* os_,
    const std::string& prefix_) :
  os(os_),
  prefix(prefix_) {
  assert(os != nullptr);

}

TestMarkFormatterIOS::~TestMarkFormatterIOS() = default;

void TestMarkFormatterIOS::printLine(
    const std::string& prefix_,
    const std::string& label_,
    const TestMark* mark_,
    int indent_,
    bool open_) {
  std::ostringstream oss_;
  oss_ << prefix << prefix_;
  for(int i_(0); i_ < indent_; ++i_)
    oss_ << "  ";
  if(open_) {
    if(!label_.empty())
      oss_ << label_ << ": ";
    mark_->printOpen(*os, oss_.str());
  }
  else
    mark_->printClose(*os, oss_.str());
}

void TestMarkFormatterIOS::setTextForeground(
    Color color_) {
  /* -- default empty implementation */
}

void TestMarkFormatterIOS::setTextStyle(
    Style style_) {
  /* -- default empty implementation */
}

void TestMarkFormatterIOS::resetTextAttributes() {
  /* -- default empty implementation */
}

void TestMarkFormatterIOS::openMark(
    const std::string& label_,
    const TestMark* mark_,
    int indent_) {
  printLine("  ", label_, mark_, indent_, true);
}

void TestMarkFormatterIOS::closeMark(
    const std::string& label_,
    const TestMark* mark_,
    int indent_) {
  printLine("  ", label_, mark_, indent_, false);
}

void TestMarkFormatterIOS::openDeleted(
    const std::string& label_,
    const TestMark* mark_,
    int indent_) {
  setTextForeground(Color::RED);
  printLine("- ", label_, mark_, indent_, true);
  resetTextAttributes();
}

void TestMarkFormatterIOS::closeDeleted(
    const std::string& label_,
    const TestMark* mark_,
    int indent_) {
  setTextForeground(Color::RED);
  printLine("- ", label_, mark_, indent_, false);
  resetTextAttributes();
}

void TestMarkFormatterIOS::openAdded(
    const std::string& label_,
    const TestMark* mark_,
    int indent_) {
  setTextForeground(Color::GREEN);
  printLine("+ ", label_, mark_, indent_, true);
  resetTextAttributes();
}

void TestMarkFormatterIOS::closeAdded(
    const std::string& label_,
    const TestMark* mark_,
    int indent_) {
  setTextForeground(Color::GREEN);
  printLine("+ ", label_, mark_, indent_, false);
  resetTextAttributes();
}

void TestMarkFormatterIOS::printSeparator() {
  setTextStyle(Style::DIM);
  *os << "  ......... \n";
  resetTextAttributes();
}

} /* -- namespace OTest2 */

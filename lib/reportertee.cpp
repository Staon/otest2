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

#include <reportertee.h>

#include <algorithm>
#include <assert.h>
#include <functional>

namespace OTest2 {

using namespace std::placeholders;

ReporterTee::ReporterTee() {

}

ReporterTee::~ReporterTee() {

}

void ReporterTee::appendReporter(
    Reporter* reporter_) {
  assert(reporter_ != nullptr);

  reporters.push_back(reporter_);
}

void ReporterTee::enterTest(
    const Context& context_,
    const std::string& name_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterTest,
          _1,
          std::cref(context_),
          std::cref(name_)));
}

void ReporterTee::enterSuite(
    const Context& context_,
    const std::string& name_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterSuite,
          _1,
          std::cref(context_),
          std::cref(name_)));
}

void ReporterTee::enterCase(
    const Context& context_,
    const std::string& name_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterCase,
          _1,
          std::cref(context_),
          std::cref(name_)));
}

void ReporterTee::enterState(
    const Context& context_,
    const std::string& name_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterState,
          _1,
          std::cref(context_),
          std::cref(name_)));
}

void ReporterTee::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& message_,
    const std::string& file_,
    int lineno_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterAssert,
          _1,
          std::cref(context_),
          condition_,
          std::cref(message_),
          std::cref(file_),
          lineno_));
}

void ReporterTee::enterError(
    const Context& context_,
    const std::string& message_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterError,
          _1,
          std::cref(context_),
          std::cref(message_)));
}

void ReporterTee::reportAssertionMessage(
    const Context& context_,
    const std::string& message_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::reportAssertionMessage,
          _1,
          std::cref(context_),
          std::cref(message_)));
}

void ReporterTee::leaveAssert(
    const Context& context_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveAssert,
          _1,
          std::cref(context_)));
}

void ReporterTee::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveState,
          _1,
          std::cref(context_),
          std::cref(name_),
          result_));
}

void ReporterTee::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveCase,
          _1,
          std::cref(context_),
          std::cref(name_),
          result_));
}

void ReporterTee::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveSuite,
          _1,
          std::cref(context_),
          std::cref(name_),
          result_));
}

void ReporterTee::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveTest,
          _1,
          std::cref(context_),
          std::cref(name_),
          result_));
}

} /* -- namespace OTest2 */

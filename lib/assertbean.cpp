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

#include <assertbean.h>

#include <algorithm>

namespace OTest2 {

AssertBean::AssertBean() :
  condition(false),
  message() {

}

AssertBean::AssertBean(
    bool condition_,
    const std::string& message_) :
  condition(condition_),
  message(message_) {

}

AssertBean::AssertBean(
    const AssertBean& src_) :
  condition(src_.condition),
  message(src_.message) {

}

AssertBean::~AssertBean() {

}

void AssertBean::swap(
    AssertBean& b2_) noexcept {
  std::swap(condition, b2_.condition);
  message.swap(b2_.message);
}

AssertBean& AssertBean::operator = (
    const AssertBean& src_) {
  AssertBean tmp_(src_);
  swap(tmp_);
  return *this;
}

bool AssertBean::getCondition() const {
  return condition;
}

const std::string& AssertBean::getMessage() const {
  return message;
}

} /* namespace OTest2 */

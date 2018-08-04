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

#include <contextobject.h>

#include <iostream>

namespace OTest2 {

ContextObject::ContextObject() {

}

ContextObject::~ContextObject() {

}

void ContextObject::otest2AssertionGeneric(
    const std::string& file_,
    int lineno_,
    const std::string& expression_,
    const std::string& message_,
    bool condition_) {
  std::cout << "assert: " << file_ << " " << lineno_
      << " '" << expression_ << "' " << message_ << ": "
      << condition_ << std::endl;
}

void ContextObject::testAssert(
    const std::string& file_,
    int lineno_,
    const std::string& expression_,
    bool condition_) {
  otest2AssertionGeneric(file_, lineno_, expression_, "", condition_);
}

} /* namespace OTest2 */

/*
 * Copyright (C) 2020 Ondrej Starek
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

#include "initializer.h"

#include <assert.h>
#include <iostream>

#include "filereader.h"

namespace OTest2 {

Initializer::Initializer(
    InitializerType type_,
    const Location& begin_,
    const Location& end_) :
  type(type_),
  begin(begin_),
  end(end_),
  initializer_text() {

}

Initializer::~Initializer() {

}

void Initializer::materializeInitializer(
    FileReader& reader_) {
  initializer_text = reader_.getPart(begin, end);
}

void Initializer::printVarInitialization(
    std::ostream& os_,
    const std::string& name_) const {
  switch(type) {
    case InitializerType::CALL_INIT:
      os_ << name_ << '(' << initializer_text << ')';
      break;
    case InitializerType::LIST_INIT:
      os_ << name_ << '{' << initializer_text << '}';
      break;
    case InitializerType::LIST_INIT_ARG:
      os_ << name_ << initializer_text;
      break;
    default:
      assert(false);
      break;
  }
}

void Initializer::printArgInitialization(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << initializer_text;
}

} /* -- namespace OTest2 */

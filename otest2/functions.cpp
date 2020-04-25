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

#include "functions.h"

#include <assert.h>
#include <iostream>

#include "formatting.h"

namespace OTest2 {

Functions::Functions(
    FunctionsPtr prev_level_) :
  prev_level(prev_level_) {

}

Functions::~Functions() {

}

FunctionsPtr Functions::getPrevLevel() const {
  return prev_level;
}

void Functions::appendFixture(
    FunctionPtr start_up_,
    FunctionPtr tear_down_) {
  start_ups.push_back(start_up_);
  tear_downs.push_back(tear_down_);
}

void Functions::generateMarshalers(
    std::ostream& os_,
    int indent_,
    const std::string& classname_) const {
  for(const auto& function_ : start_ups) {
    if(function_ != nullptr) {
      Formatting::printIndent(os_, indent_);
      function_->generateMarshaler(os_, indent_, classname_);
      os_ << "\n\n";
    }
  }
  for(const auto& function_ : tear_downs) {
    if(function_ != nullptr) {
      Formatting::printIndent(os_, indent_);
      function_->generateMarshaler(os_, indent_, classname_);
      os_ << "\n\n";
    }
  }
}

void Functions::generateRegistration(
    std::ostream& os_,
    int indent_,
    const std::string& classname_) const {
  assert(start_ups.size() == tear_downs.size());

  for(int i_(0); i_ < start_ups.size(); ++i_) {
    Formatting::printIndent(os_, indent_);
    os_ << "registerFixture(\n";
    Formatting::printIndent(os_, indent_ + 2);
    if(start_ups[i_] != nullptr)
      start_ups[i_]->generateRegistration(os_, indent_ + 2, classname_);
    else
      os_ << "std::make_shared< ::OTest2::EmptyMarshaler >()";
    os_ << ",\n";
    Formatting::printIndent(os_, indent_ + 2);
    if(tear_downs[i_] != nullptr)
      tear_downs[i_]->generateRegistration(os_, indent_ + 2, classname_);
    else
      os_ << "std::make_shared< ::OTest2::EmptyMarshaler >()";
    os_ << ");\n";
  }
}

} /* -- namespace OTest2 */

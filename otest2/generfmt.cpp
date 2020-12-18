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
#include "generfmt.h"

#include <iostream>

#include "formatting.h"

namespace OTest2 {

std::ostream& writeCString(
    std::ostream& os_,
    const std::string& text_) {
  os_ << '"';
  for(char c_ : text_) {
    switch(c_) {
      case '"': os_ << "\\\""; break;
      case '\\': os_ << "\\\\"; break;
      case '\n': os_ << "\\\n"; break;
      default: os_ << c_; break;
    }
  }
  os_ << '"';

  return os_;
}

std::ostream& writeTags(
    std::ostream& os_,
    const Parser::ObjectTags& tags_,
    int indent_) {
  os_ << "::OTest2::Tags({";
  bool first_(true);
  for(const auto& tag_ : tags_) {
    if(!first_)
      os_ << ", ";
    else
      first_ = false;
    writeCString(os_, tag_);
  }
  os_ << "})";

  return os_;
}

} /* -- namespace OTest2 */

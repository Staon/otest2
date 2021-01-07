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

#include "excparser.h"

#include <sstream>
#include <utility>

namespace OTest2 {

ParserException::ParserException() :
  message(),
  file(),
  begin_line(0),
  begin_column(0),
  end_line(0),
  end_column(0) {

}

ParserException::ParserException(
    ParserException&& exc_) noexcept :
  Exception(std::move(exc_)),
  message(std::move(exc_.message)),
  file(std::move(exc_.file)),
  begin_line(exc_.begin_line),
  begin_column(exc_.begin_column),
  end_line(exc_.end_line),
  end_column(exc_.end_column) {

}

ParserException::~ParserException() {

}

void ParserException::setException(
    const std::string& message_,
    const std::string& file_,
    int begin_line_,
    int begin_column_,
    int end_line_,
    int end_column_) {
  message = message_;
  file = file_;
  begin_line = begin_line_;
  begin_column = begin_column_;
  end_line = end_line_;
  end_column = end_column_;
}

std::string ParserException::reason() const {
  std::ostringstream sos_;
  sos_ << file << '(' << begin_line << '.' << begin_column << '-' << end_line
      << '.' << end_column << "): " << message;
  return sos_.str();
}

} /* -- namespace OTest2 */

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

#include "excreader.h"

#include <sstream>
#include <utility>

namespace OTest2 {

ReaderException::ReaderException(
    const std::string& message_,
    int line_,
    int column_) :
  message(message_),
  line(line_),
  column(column_) {

}

ReaderException::ReaderException(
    ReaderException&& exc_) noexcept :
  message(std::move(exc_.message)),
  line(exc_.line),
  column(exc_.column) {

}

ReaderException::~ReaderException() {

}

std::string ReaderException::reason() const {
  std::ostringstream sos_;
  sos_ << "reader exception: (" << line << "." << column << "): " << message;
  return sos_.str();
}

} /* namespace OTest2 */

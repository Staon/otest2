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

#include "error.h"

#include <utility>

namespace OTest2 {

Error::Error(
    const std::string message_) :
  message(message_) {

}

Error::Error(
    Error&& exc_) :
  Exception(std::forward<Exception>(exc_)),
  message(std::move(exc_.message)) {

}

Error::~Error() {

}

std::string Error::reason() const {
  return "Generic error: " + message;
}

} /* namespace OTest2 */

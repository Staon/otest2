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

#include <exctestmarkin.h>

#include <utility>

namespace OTest2 {

ExcTestMarkIn::ExcTestMarkIn(
    const std::string& message_) :
  message(message_) {

}

ExcTestMarkIn::ExcTestMarkIn(
    ExcTestMarkIn&& exc_) noexcept :
  Exception(std::move(exc_)),
  message(std::move(exc_.message)) {

}

ExcTestMarkIn::~ExcTestMarkIn() {

}

std::string ExcTestMarkIn::reason() const {
  return message;
}

} /* -- namespace OTest2 */

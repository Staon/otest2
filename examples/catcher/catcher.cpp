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

#include "catcher.h"

#include <exception>

#include "myexception.h"

namespace OTest2 {

namespace Examples {

Catcher::Catcher() {

}

Catcher::~Catcher() {

}

bool Catcher::catchException(
    const Context& context_,
    std::function<void(const Context&)> ftor_,
    std::string& message_) noexcept {
  try {
    ftor_(context_);
    return false;
  }
  catch(MyException& exc_) {
    message_ = exc_.getMessage();
    return true;
  }
  catch(std::exception& exc_) {
    message_ = exc_.what();
    return true;
  }
  catch(const char* msg_) {
    message_ = msg_;
    return true;
  }
  catch(...) {
    message_ = "unknown exception";
    return true;
  }
}

} /* -- namespace Examples */

} /* -- namespace OTest2 */

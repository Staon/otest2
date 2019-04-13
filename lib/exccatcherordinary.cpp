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

#include <exccatcherordinary.h>

#include <exception>
#include <sstream>

namespace OTest2 {

ExcCatcherOrdinary::ExcCatcherOrdinary() {

}

ExcCatcherOrdinary::~ExcCatcherOrdinary() {

}

bool ExcCatcherOrdinary::catchException(
    const Context& context_,
    std::function<void(const Context&)> ftor_,
    std::string& message_) noexcept {
  /* -- run the functor */
  try {
    ftor_(context_);
  }
  catch(std::exception& exc_) {
    std::ostringstream sos_;
    sos_ << "unexpected exception: " << exc_.what();
    message_ = sos_.str();
    return true;
  }
  catch(...) {
    message_ = "unexpected unknown exception";
    return true;
  }

  return false;
}

} /* namespace OTest2 */

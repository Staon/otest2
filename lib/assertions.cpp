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

#include <assertions.h>

#include <string>

#include <assertbean.h>
#include <context.h>
#include <exccatcher.h>

namespace OTest2 {

bool GenericAssertion::testAssert(
    bool condition_) {
  return simpleAssertionImpl(condition_, "", true);
}

bool GenericAssertion::testAssert(
    const AssertBean& bean_) {
  return simpleAssertionImpl(bean_.getCondition(), bean_.getMessage(), true);
}

bool GenericAssertion::testException(
    std::function<bool()> ftor_) {
  std::string message_;
  bool result_(false);
  if(otest2Context().exception_catcher->catchException(
      otest2Context(),
      [&result_, ftor_](const Context&) {
        result_ = ftor_();
      },
      message_)) {
    return simpleAssertionImpl(false, message_, false);
  }
  if(result_) {
    return simpleAssertionImpl(
        true,
        "the expected exception has been caught",
        false);
  }
  else {
    return simpleAssertionImpl(
        false,
        "an exception was expected but no one has occurred",
        false);
  }
}

}  /* -- namespace OTest2 */

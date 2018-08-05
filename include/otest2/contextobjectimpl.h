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


#ifndef OTest2__INCLUDE_OTEST2_CONTEXTOBJECTIMPL_H_
#define OTest2__INCLUDE_OTEST2_CONTEXTOBJECTIMPL_H_

#include <otest2/contextobject.h>

#include <sstream>
#include <string>

namespace OTest2 {

template<typename A_, typename B_>
void ContextObject::testAssertEqual(
    const std::string& file_,
    int lineno_,
    A_ expected_,
    B_ actual_) {
  std::ostringstream sos_;
  sos_ << "expected: " << expected_ << ", actual: " << actual_;
  otest2AssertGeneric(
      file_, lineno_, "", sos_.str(), expected_ == actual_);
}

} /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CONTEXTOBJECTIMPL_H_ */

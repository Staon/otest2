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
#ifndef OTest2__INCLUDE_OTEST2_ASSERTIONSIMPL_H_
#define OTest2__INCLUDE_OTEST2_ASSERTIONSIMPL_H_

#include <otest2/assertions.h>

#include <sstream>

#include <otest2/printtraits.h>

namespace OTest2 {

template<typename Compare_, typename A_, typename B_>
bool GenericAssertion::testAssertCompare(
    A_ a_,
    B_ b_) {
  Compare_ cmp_;
  std::ostringstream sos_;
  sos_ << "relation 'a ";
  PrintTrait<Compare_>::print(sos_, cmp_);
  sos_ << " b' has failed: a = ";
  PrintTrait<A_>::print(sos_, a_);
  sos_ << ", b = ";
  PrintTrait<B_>::print(sos_, b_);
  return simpleAssertionImpl(cmp_(a_, b_), sos_.str(), false);
}

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTIONSIMPL_H_ */

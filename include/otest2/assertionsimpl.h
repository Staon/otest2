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
#include <type_traits>

#include <typeinfo>
#include <cxxabi.h>

#include <otest2/printtraits.h>

namespace OTest2 {

template<typename Compare_, typename A_, typename B_>
bool GenericAssertion::testAssertCompare(
    A_&& a_,
    B_&& b_) {
  typedef typename std::remove_const<typename std::remove_reference<A_>::type>::type AType_;
  typedef typename std::remove_const<typename std::remove_reference<B_>::type>::type BType_;

  Compare_ cmp_;
  const bool condition_(cmp_(a_, b_));

  /* -- report the result and the used relation operator */
  std::ostringstream sos_;
  if(condition_) {
    sos_ << "check 'a ";
    PrintTrait<Compare_>::print(sos_, cmp_);
    sos_ << " b' has passed";
  }
  else {
    sos_ << "check 'a ";
    PrintTrait<Compare_>::print(sos_, cmp_);
    sos_ << " b' has failed";
  }
  enterAssertion(condition_, sos_.str(), false);

  /* -- report values of the operands */
  sos_.str("");
  sos_ << "a = ";

//  int status_;
//  char* realname_(abi::__cxa_demangle(typeid(AType_).name(), 0, 0, &status_));
//  sos_ << realname_ << " ";
//  ::free(realname_);

  PrintTrait<typename NormalizeStringType<AType_>::Type>::print(sos_, a_);
  assertionMessage(condition_, sos_.str());
  sos_.str("");
  sos_ << "b = ";
  PrintTrait<typename NormalizeStringType<BType_>::Type>::print(sos_, b_);
  assertionMessage(condition_, sos_.str());

  return leaveAssertion(condition_);
}

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTIONSIMPL_H_ */

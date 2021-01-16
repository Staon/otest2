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

#include <type_traits>

#include <typeinfo>
#include <cxxabi.h>

#include <otest2/assertstream.h>
#include <otest2/printtraits.h>

namespace OTest2 {

template<template<typename, typename> class Compare_, typename A_, typename B_>
bool GenericAssertion::testAssertCompare(
    A_&& a_,
    B_&& b_) {
  typedef typename AssertionParameter<A_>::Type AType_;
  typedef typename AssertionParameter<B_>::Type BType_;
  typedef Compare_<AType_, BType_> CmpType_;

  /* -- compare the values */
  CmpType_ cmp_;
  const bool condition_(cmp_(a_, b_));

  /* -- report the result and the used relation operator */
  AssertStream report_(enterAssertion(condition_));
  if(condition_) {
    report_ << "check 'a ";
    PrintTrait<CmpType_>::print(report_, cmp_);
    report_ << " b' has passed" << commitMsg();
  }
  else {
    report_ << "check 'a ";
    PrintTrait<CmpType_>::print(report_, cmp_);
    report_ << " b' has failed" << commitMsg();
  }

  /* -- report values of the operands */
  report_ << "a = ";
  PrintTrait<AType_>::print(report_, a_);
  report_ << commitMsg();
  report_ << "b = ";
  PrintTrait<BType_>::print(report_, b_);
  report_ << commitMsg();

  return report_.getResult();
}

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTIONSIMPL_H_ */

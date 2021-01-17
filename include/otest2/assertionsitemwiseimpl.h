/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2
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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTIONSITEMWISEIMPL_H_
#define OTest2_INCLUDE_OTEST2_ASSERTIONSITEMWISEIMPL_H_

#include <otest2/assertionsitemwise.h>

#include <assert.h>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <otest2/assertstream.h>
#include <otest2/comparisons.h>
#include <otest2/containertrait.h>
#include <otest2/printtraits.h>
#include <otest2/printutils.h>
#include <otest2/typetraits.h>

namespace OTest2 {

namespace Private {

template<
    template<typename, typename> class Compare_,
    typename IterA_,
    typename IterB_>
bool compareListsItemWise(
    std::vector<std::string>& messages_,
    IterA_ begin_a_,
    IterA_ end_a_,
    IterB_ begin_b_,
    IterB_ end_b_) {
  typedef typename AssertionParameter<decltype(*begin_a_)>::Type AType_;
  typedef typename AssertionParameter<decltype(*begin_b_)>::Type BType_;
  typedef Compare_<AType_, BType_> Comparator_;

  /* -- iterate and check items */
  Comparator_ cmp_;
  int index_(0);
  while(begin_a_ != end_a_ && begin_b_ != end_b_) {
    bool condition_(cmp_(*begin_a_, *begin_b_));
    if(!condition_) {
      /* -- report the failure */
      std::ostringstream sos_;
      sos_ << "the ";
      printOrdinalNumber(sos_, index_);
      sos_ << " item has failed: a[" << index_ << "] ";
      PrintTrait<Comparator_>::print(sos_, cmp_);
      sos_ << " b[" << index_ << "]";
      messages_.push_back(sos_.str());

      sos_.str("");
      sos_ << "a[" << index_ << "] = ";
      PrintTrait<AType_>::print(sos_, *begin_a_);
      messages_.push_back(sos_.str());

      sos_.str("");
      sos_ << "b[" << index_ << "] = ";
      PrintTrait<BType_>::print(sos_, *begin_b_);
      messages_.push_back(sos_.str());

      return false;
    }

    /* -- move to next item */
    ++begin_a_;
    ++begin_b_;
    ++index_;
  }

  /* -- there are some other items in the left container */
  if(begin_a_ != end_a_) {
    messages_.push_back("left container is bigger then the right one");
    return false;
  }

  /* -- there are some other items in the right container */
  if(begin_b_ != end_b_) {
    messages_.push_back("left container is smaller then the right one");
    return false;
  }

  messages_.push_back("the check has passed");
  return true;
}

} /* -- namespace Private */

template<
    template<typename, typename> class Compare_,
    typename IterA_,
    typename IterB_>
bool ItemWiseAssertion::testAssertItemWiseList(
    IterA_ begin_a_,
    IterA_ end_a_,
    IterB_ begin_b_,
    IterB_ end_b_) {
  /* -- check the container */
  std::vector<std::string> messages_;
  bool result_(Private::compareListsItemWise<Compare_>(
      messages_, begin_a_, end_a_, begin_b_, end_b_));
  assert(messages_.size() > 0);

  /* -- report the result */
  AssertStream report_(enterAssertion(result_));
  for(const auto& message_ : messages_)
    report_ << message_ << commitMsg();
  return report_.getResult();
}

template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
bool ItemWiseAssertion::testAssertItemWise(
    IterA_ begin_a_,
    IterA_ end_a_,
    IterB_ begin_b_,
    IterB_ end_b_) {
  return testAssertItemWiseList<Compare_>(begin_a_, end_a_, begin_b_, end_b_);
}

template<template<typename, typename> class Compare_, typename ContainerA_, typename IterB_>
bool ItemWiseAssertion::testAssertItemWise(
    const ContainerA_& a_,
    IterB_ begin_b_,
    IterB_ end_b_) {
  return testAssertItemWiseList<Compare_>(
      ListContainerTrait<ContainerA_>::begin(a_),
      ListContainerTrait<ContainerA_>::end(a_),
      begin_b_,
      end_b_);
}

template<template<typename, typename> class Compare_, typename IterA_, typename ContainerB_>
bool ItemWiseAssertion::testAssertItemWise(
    IterA_ begin_a_,
    IterA_ end_a_,
    const ContainerB_& b_) {
  return testAssertItemWiseList<Compare_>(
      begin_a_,
      end_a_,
      ListContainerTrait<ContainerB_>::begin(b_),
      ListContainerTrait<ContainerB_>::end(b_));
}

template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool ItemWiseAssertion::testAssertItemWise(
    const ContainerA_& a_,
    const ContainerB_& b_) {
  return testAssertItemWiseList<Compare_>(
      ListContainerTrait<ContainerA_>::begin(a_),
      ListContainerTrait<ContainerA_>::end(a_),
      ListContainerTrait<ContainerB_>::begin(b_),
      ListContainerTrait<ContainerB_>::end(b_));
}

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTIONSITEMWISEIMPL_H_ */

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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTIONSMAPIMPL_H_
#define OTest2_INCLUDE_OTEST2_ASSERTIONSMAPIMPL_H_

#include <otest2/assertionsmap.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <otest2/containertrait.h>
#include <otest2/printtraits.h>
#include <otest2/typetraits.h>

namespace OTest2 {

namespace Private {

template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool compareMaps(
    std::vector<std::string>& messages_,
    const ContainerA_& a_,
    const ContainerB_& b_) {
  typedef typename AssertionParameter<typename MapContainerTrait<ContainerA_>::KeyType>::Type AKey_;
  typedef typename AssertionParameter<typename MapContainerTrait<ContainerA_>::ValueType>::Type AValue_;
  typedef typename AssertionParameter<typename MapContainerTrait<ContainerB_>::KeyType>::Type BKey_;
  typedef typename AssertionParameter<typename MapContainerTrait<ContainerB_>::ValueType>::Type BValue_;
  typedef Compare_<AValue_, BValue_> Comparator_;

  Comparator_ cmp_;

  std::ostringstream first_line_;
  first_line_ << "the assertion a ";
  PrintTrait<Comparator_>::print(first_line_, cmp_);
  first_line_ << " b has ";

  /* -- compare left map against the right one */
  auto iter_a_(MapContainerTrait<ContainerA_>::begin(a_));
  auto end_a_(MapContainerTrait<ContainerA_>::end(a_));
  while(iter_a_ != end_a_) {
    /* -- find the key in the second map */
    auto range_b_(MapContainerTrait<ContainerB_>::equalRange(b_, (*iter_a_).first));
    if(range_b_.first == range_b_.second) {
      /* -- the key is not found in the second map */
      std::ostringstream sos_;
      sos_ << first_line_.str() << "failed: the item <";
      PrintTrait<AKey_>::print(sos_, (*iter_a_).first);
      sos_ << ", ";
      PrintTrait<AValue_>::print(sos_, (*iter_a_).second);
      sos_ << "> is not present in the map b";

      messages_.push_back(sos_.str());
      return false;
    }

    /* -- compare subsequent values with the same key */
    while(true) {
      if(iter_a_ == end_a_
         || !MapContainerTrait<ContainerA_>::keyEqual(a_, (*iter_a_).first, (*range_b_.first).first)) {
        /* -- the subsequence is shorter in the map a */
        std::ostringstream sos_;
        sos_ << first_line_.str() << "failed: the subsequence of items with the key ";
        PrintTrait<BKey_>::print(sos_, (*range_b_.first).first);
        sos_ << " is shorter in the map a than in the map b";
        messages_.push_back(sos_.str());

        return false;
      }

      if(!cmp_((*iter_a_).second, (*range_b_.first).second)) {
        /* -- the values dont't fit operator */
        std::ostringstream sos_;
        sos_ << first_line_.str() << "failed: check a[";
        PrintTrait<AKey_>::print(sos_, (*iter_a_).first);
        sos_ << "] ";
        PrintTrait<Comparator_>::print(sos_, cmp_);
        sos_ << " b[";
        PrintTrait<BKey_>::print(sos_, (*range_b_.first).first);
        sos_ << "] has failed";
        messages_.push_back(sos_.str());

        sos_.str("");
        sos_ << "a[";
        PrintTrait<AKey_>::print(sos_, (*iter_a_).first);
        sos_ << "] = ";
        PrintTrait<AValue_>::print(sos_, (*iter_a_).second);
        messages_.push_back(sos_.str());

        sos_.str("");
        sos_ << "b[";
        PrintTrait<AKey_>::print(sos_, (*range_b_.first).first);
        sos_ << "] = ";
        PrintTrait<AValue_>::print(sos_, (*range_b_.first).second);
        messages_.push_back(sos_.str());

        return false;
      }

      /* -- move to next item in the equality range */
      ++range_b_.first;
      if(range_b_.first == range_b_.second) {
        /* -- end of the range, check the end of the range in the map A */
        auto iter_a_tmp_(iter_a_);
        ++iter_a_;
        if(iter_a_ == end_a_
           || !MapContainerTrait<ContainerA_>::keyEqual(a_, (*iter_a_).first, (*iter_a_tmp_).first)) {
          /* -- end of the subsequence in the map A -> OK */
          break;
        }

        std::ostringstream sos_;
        sos_ << first_line_.str() << "failed: the subsequence of items with the key ";
        PrintTrait<AKey_>::print(sos_, (*iter_a_).first);
        sos_ << " is longer in the map a than in the map b";
        messages_.push_back(sos_.str());

        return false;
      }

      /* -- move to next item in the subsequence */
      ++iter_a_;
    }
  }

  /* -- Compare right map against the left one. Check just existence
   *    of keys as the key subsequences are already checked. */
  auto iter_b_(MapContainerTrait<ContainerB_>::begin(b_));
  auto end_b_(MapContainerTrait<ContainerB_>::end(b_));
  while(iter_b_ != end_b_) {
    auto range_a_(MapContainerTrait<ContainerA_>::equalRange(a_, (*iter_b_).first));
    if(range_a_.first == range_a_.second) {
      /* -- the key is not found in the first map */
      std::ostringstream sos_;
      sos_ << first_line_.str() << "failed: the item <";
      PrintTrait<BKey_>::print(sos_, (*iter_b_).first);
      sos_ << ", ";
      PrintTrait<BValue_>::print(sos_, (*iter_b_).second);
      sos_ << "> is not present in the map a";

      messages_.push_back(sos_.str());
      return false;
    }

    /* -- move to next item */
    ++iter_b_;
  }

  std::ostringstream sos_;
  sos_ << first_line_.str() << "passed";
  messages_.push_back(sos_.str());

  return true;
}

} /* -- namespace Private */

template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool MapAssertion::testAssertMap(
    const ContainerA_& a_,
    const ContainerB_& b_) {
  std::vector<std::string> messages_;
  bool result_(Private::compareMaps<Compare_>(messages_, a_, b_));
  assert(!messages_.empty());

  enterAssertion(result_, messages_[0], false);
  for(int i_(1); i_ < messages_.size(); ++i_)
    assertionMessage(result_, messages_[i_]);
  return leaveAssertion(result_);
}

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTIONSMAPIMPL_H_ */

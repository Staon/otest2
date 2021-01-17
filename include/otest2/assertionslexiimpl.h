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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTIONSLEXIIMPL_H_
#define OTest2_INCLUDE_OTEST2_ASSERTIONSLEXIIMPL_H_

#include <otest2/assertionslexi.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <otest2/assertstream.h>
#include <otest2/containertrait.h>
#include <otest2/printtraits.h>
#include <otest2/printutils.h>
#include <otest2/typetraits.h>

namespace OTest2 {

namespace Private {

template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
bool compareListsLexicographically(
    std::vector<std::string>& messages_,
    IterA_ begin_a_,
    IterA_ end_a_,
    IterB_ begin_b_,
    IterB_ end_b_) {
  typedef typename AssertionParameter<decltype(*begin_a_)>::Type AType_;
  typedef typename AssertionParameter<decltype(*begin_b_)>::Type BType_;
  typedef Compare_<AType_, BType_> Comparator_;

  int index_(0);
  Comparator_ cmp_;
  std::ostringstream sos_;
  while(begin_a_ != end_a_ && begin_b_ != end_b_) {
    int cmp_result_(cmp_.checkItems(*begin_a_, *begin_b_));
    if(cmp_result_ < 0) {
      /* -- the a_ list is lesser */
      sos_ << "the assertion 'a ";
      PrintTrait<Comparator_>::print(sos_, cmp_);
      sos_ << " b' has passed";
      messages_.push_back(sos_.str());
      return true;
    }
    if(cmp_result_ > 0) {
      /* -- the a_ list is greater */
      sos_ << "the ";
      printOrdinalNumber(sos_, index_);
      sos_ << " items have failed: a[" << index_ << "] ";
      PrintTrait<Comparator_>::print(sos_, cmp_);
      sos_ << " b[" << index_ << "]";
      messages_.push_back(sos_.str());

      sos_.str("");
      sos_ << "a[" << index_ << "] = ";
      PrintTrait<AType_>::print(sos_, *begin_a_);
      messages_.push_back(sos_.str());

      sos_.str("");
      sos_ << "b[" << index_ << "] = ";
      PrintTrait<AType_>::print(sos_, *begin_b_);
      messages_.push_back(sos_.str());

      return false;
    }

    /* -- move to next item */
    ++begin_a_;
    ++begin_b_;
    ++index_;
  }

  /* -- All items equal each other in the pair. The result is defined by
   *    length of resting lists - whether a list is longer then the other
   *    or shorter or the same length. */
  const bool a_ends_(begin_a_ == end_a_);
  const bool b_ends_(begin_b_ == end_b_);
  if(cmp_.checkRests(a_ends_, b_ends_)) {
    sos_ << "the assertion 'a ";
    PrintTrait<Comparator_>::print(sos_, cmp_);
    sos_ << " b' has passed: one list is the prefix of the other one";
    messages_.push_back(sos_.str());
    return true;
  }

  /* -- The lists doesn't match the operator */
  sos_ << "the assertion 'a ";
  PrintTrait<Comparator_>::print(sos_, cmp_);
  sos_ << " b' has failed.";
  messages_.push_back(sos_.str());

  if(a_ends_ && b_ends_) {
    messages_.push_back("the lists are the same length");
  }
  else if(a_ends_) {
    messages_.push_back("the list a is shorter");
  }
  else {
    messages_.push_back("the list a is longer");
  }
  return false;
}

} /* -- namespace Private */

template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
bool LexicographicalAssertion::testAssertLexiList(
    IterA_ begin_a_,
    IterA_ end_a_,
    IterB_ begin_b_,
    IterB_ end_b_) {
  /* -- check the container */
  std::vector<std::string> messages_;
  bool result_(Private::compareListsLexicographically<Compare_>(
      messages_, begin_a_, end_a_, begin_b_, end_b_));
  assert(messages_.size() > 0);

  /* -- report the result */
  AssertStream report_(enterAssertion(result_));
  for(const auto& message_ : messages_)
    report_ << message_ << commitMsg();
  return report_.getResult();
}

template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
bool LexicographicalAssertion::testAssertLexi(
    IterA_ begin_a_,
    IterA_ end_a_,
    IterB_ begin_b_,
    IterB_ end_b_) {
  return testAssertLexiList<Compare_>(begin_a_, end_a_, begin_b_, end_b_);
}

template<template<typename, typename> class Compare_, typename ContainerA_, typename IterB_>
bool LexicographicalAssertion::testAssertLexi(
    const ContainerA_& a_,
    IterB_ begin_b_,
    IterB_ end_b_) {
  return testAssertLexiList<Compare_>(
      ListContainerTrait<ContainerA_>::begin(a_),
      ListContainerTrait<ContainerA_>::end(a_),
      begin_b_,
      end_b_);
}

template<template<typename, typename> class Compare_, typename IterA_, typename ContainerB_>
bool LexicographicalAssertion::testAssertLexi(
    IterA_ begin_a_,
    IterA_ end_a_,
    const ContainerB_& b_) {
  return testAssertLexiList<Compare_>(
      begin_a_,
      end_a_,
      ListContainerTrait<ContainerB_>::begin(b_),
      ListContainerTrait<ContainerB_>::end(b_));
}

template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool LexicographicalAssertion::testAssertLexi(
    const ContainerA_& a_,
    const ContainerB_& b_) {
  return testAssertLexiList<Compare_>(
      ListContainerTrait<ContainerA_>::begin(a_),
      ListContainerTrait<ContainerA_>::end(a_),
      ListContainerTrait<ContainerB_>::begin(b_),
      ListContainerTrait<ContainerB_>::end(b_));
}

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTIONSLEXIIMPL_H_ */

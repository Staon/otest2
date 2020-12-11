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

#ifndef OTest2_INCLUDE_OTEST2_COMPARISONSLEXI_H_
#define OTest2_INCLUDE_OTEST2_COMPARISONSLEXI_H_

#include <otest2/comparisons.h>
#include <otest2/typetraits.h>

namespace OTest2 {

/**
 * @brief the '<' operator for lexicographical assertions
 */
template<typename A_, typename B_>
class LexiLess {
  private:
    Less<A_, B_> less;
    Greater<A_, B_> greater;

  public:
    int checkItems(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      if(less(a_, b_))
        return -1;
      if(greater(a_, b_))
        return 1;
      return 0;
    }

    bool checkRests(
        bool a_ends_,
        bool b_ends_) const {
      return a_ends_ && !b_ends_;
    }
};

/**
 * @brief the '<=' operator for lexicographical assertions
 */
template<typename A_, typename B_>
class LexiLessOrEqual {
  private:
    Less<A_, B_> less;
    Greater<A_, B_> greater;

  public:
    int checkItems(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      if(less(a_, b_))
        return -1;
      if(greater(a_, b_))
        return 1;
      return 0;
    }

    bool checkRests(
        bool a_ends_,
        bool b_ends_) const {
      return a_ends_;
    }
};

/**
 * @brief the '>' operator for lexicographical assertions
 */
template<typename A_, typename B_>
class LexiGreater {
  private:
    Less<A_, B_> less;
    Greater<A_, B_> greater;

  public:
    int checkItems(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      if(less(a_, b_))
        return 1;
      if(greater(a_, b_))
        return -1;
      return 0;
    }

    bool checkRests(
        bool a_ends_,
        bool b_ends_) const {
      return !a_ends_ && b_ends_;
    }
};

/**
 * @brief the '>=' operator for lexicographical assertions
 */
template<typename A_, typename B_>
class LexiGreaterOrEqual {
  private:
    Less<A_, B_> less;
    Greater<A_, B_> greater;

  public:
    int checkItems(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      if(less(a_, b_))
        return 1;
      if(greater(a_, b_))
        return -1;
      return 0;
    }

    bool checkRests(
        bool a_ends_,
        bool b_ends_) const {
      return b_ends_;
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_COMPARISONSLEXI_H_ */

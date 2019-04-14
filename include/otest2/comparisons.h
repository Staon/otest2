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
#ifndef OTest2__INCLUDE_OTEST2_COMPARISONS_H_
#define OTest2__INCLUDE_OTEST2_COMPARISONS_H_

#include <cstring>

#include <otest2/typetraits.h>

namespace OTest2 {

template<typename A_, typename B_>
struct Equal {
    bool operator()(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      return a_ == b_;
    }
};

template<>
struct Equal<const char*, const char*> {
    bool operator()(
        const char* a_,
        const char* b_) const {
      return std::strcmp(a_, b_) == 0;
    }
};

template<typename A_, typename B_>
struct NotEqual {
    bool operator()(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      return a_ != b_;
    }
};

template<>
struct NotEqual<const char*, const char*> {
    bool operator()(
        const char* a_,
        const char* b_) const {
      return std::strcmp(a_, b_) != 0;
    }
};

template<typename A_, typename B_>
struct Less {
    bool operator()(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      return a_ < b_;
    }
};

template<>
struct Less<const char*, const char*> {
    bool operator()(
        const char* a_,
        const char* b_) const {
      return std::strcmp(a_, b_) < 0;
    }
};

template<typename A_, typename B_>
struct LessOrEqual {
    bool operator()(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      return a_ <= b_;
    }
};

template<>
struct LessOrEqual<const char*, const char*> {
    bool operator()(
        const char* a_,
        const char* b_) const {
      return std::strcmp(a_, b_) <= 0;
    }
};

template<typename A_, typename B_>
struct Greater {
    bool operator()(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      return a_ > b_;
    }
};

template<>
struct Greater<const char*, const char*> {
    bool operator()(
        const char* a_,
        const char* b_) const {
      return std::strcmp(a_, b_) > 0;
    }
};

template<typename A_, typename B_>
struct GreaterOrEqual {
    bool operator()(
        typename TypeTrait<A_>::BestArg a_,
        typename TypeTrait<B_>::BestArg b_) const {
      return a_ >= b_;
    }
};

template<>
struct GreaterOrEqual<const char*, const char*> {
    bool operator()(
        const char* a_,
        const char* b_) const {
      return std::strcmp(a_, b_) >= 0;
    }
};

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_COMPARISONS_H_ */

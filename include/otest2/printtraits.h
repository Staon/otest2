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
#ifndef OTest2__INCLUDE_OTEST2_PRINTTRAITS_H_
#define OTest2__INCLUDE_OTEST2_PRINTTRAITS_H_

#include <iosfwd>
#include <string>

#include <otest2/comparisons.h>
#include <otest2/comparisonslexi.h>
#include <otest2/typetraits.h>

namespace OTest2 {

/**
 * @brief Generic type trait
 *
 * The type traits are used for printing of assertion arguments. User
 * is supposed implementing own traits
 */
template<typename Type_>
struct PrintTrait {
    static std::ostream& print(
        std::ostream& os_,
        typename TypeTrait<Type_>::BestArg value_) {
      return os_ << value_;
    }
};

/* -- specializations for string types */
template<>
struct PrintTrait<std::string> {
    static std::ostream& print(
        std::ostream& os_,
        const std::string& value_) {
      return os_ << '"' << value_ << '"';
    }
};
template<>
struct PrintTrait<const char*> {
    static std::ostream& print(
        std::ostream& os_,
        const char* value_) {
      if(value_ == nullptr)
        return os_ << "NULL";
      else
        return os_ << '"' << value_ << '"';
    }
};

/* -- specializations for comparison functors */
template<typename A_, typename B_>
struct PrintTrait<Equal<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const Equal<A_, B_>& value_) {
      return os_ << "==";
    }
};
template<typename A_, typename B_, typename P_>
struct PrintTrait<EqualFP<A_, B_, P_> > {
    static std::ostream& print(
        std::ostream& os_,
        const EqualFP<A_, B_, P_>& value_) {
      return os_ << "==";
    }
};
template<typename A_, typename B_>
struct PrintTrait<NotEqual<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const NotEqual<A_, B_>& value_) {
      return os_ << "!=";
    }
};
template<typename A_, typename B_, typename P_>
struct PrintTrait<NotEqualFP<A_, B_, P_> > {
    static std::ostream& print(
        std::ostream& os_,
        const NotEqualFP<A_, B_, P_>& value_) {
      return os_ << "!=";
    }
};
template<typename A_, typename B_>
struct PrintTrait<Less<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const Less<A_, B_>& value_) {
      return os_ << "<";
    }
};
template<typename A_, typename B_>
struct PrintTrait<LessOrEqual<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const LessOrEqual<A_, B_>& value_) {
      return os_ << "<=";
    }
};
template<typename A_, typename B_>
struct PrintTrait<Greater<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const Greater<A_, B_>& value_) {
      return os_ << ">";
    }
};
template<typename A_, typename B_>
struct PrintTrait<GreaterOrEqual<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const GreaterOrEqual<A_, B_>& value_) {
      return os_ << ">=";
    }
};

/* -- lexicographical comparators */
template<typename A_, typename B_>
struct PrintTrait<LexiLess<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const LexiLess<A_, B_>& value_) {
      return os_ << "<";
    }
};
template<typename A_, typename B_>
struct PrintTrait<LexiLessOrEqual<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const LexiLessOrEqual<A_, B_>& value_) {
      return os_ << "<=";
    }
};
template<typename A_, typename B_>
struct PrintTrait<LexiGreater<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const LexiGreater<A_, B_>& value_) {
      return os_ << ">";
    }
};
template<typename A_, typename B_>
struct PrintTrait<LexiGreaterOrEqual<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const LexiGreaterOrEqual<A_, B_>& value_) {
      return os_ << ">=";
    }
};

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_PRINTTRAITS_H_ */

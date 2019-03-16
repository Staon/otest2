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
        Type_ value_) {
      return os_ << value_;
    }
};

/* -- specializations for string types */
template<>
struct PrintTrait<std::string> {
    static std::ostream& print(
        std::ostream& os_,
        const std::string& value_) {
      return os_ << '\'' << value_ << '\'';
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
        return os_ << '\'' << value_ << '\'';
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
template<typename A_, typename B_>
struct PrintTrait<Less<A_, B_> > {
    static std::ostream& print(
        std::ostream& os_,
        const Less<A_, B_>& value_) {
      return os_ << "<";
    }
};

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_PRINTTRAITS_H_ */

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
#ifndef OTest2__INCLUDE_OTEST2_TYPETRAITS_H_
#define OTest2__INCLUDE_OTEST2_TYPETRAITS_H_

#include <cstddef>
#include <type_traits>

namespace OTest2 {

namespace Private {

template<typename Type_, bool scalar_> struct SelectArgType;
template<typename Type_>
struct SelectArgType<Type_, true> {
    typedef Type_ Value;
};
template<typename Type_>
struct SelectArgType<Type_, false> {
    typedef const Type_& Value;
};

}  /* -- namespace Private */

/**
 * @brief Generic type trait used by the framework
 */
template<typename Type_>
struct TypeTrait {
    /** best type used for argument passing */
    typedef typename Private::SelectArgType<
        Type_,
        std::is_scalar<Type_>::value >::Value BestArg;
};

/**
 * @brief A trait which is used to normalize types of string literals
 *     and C-strings.
 */
template<typename Type_>
struct NormalizeStringType {
    typedef Type_ Type;
};
template<std::size_t len_>
struct NormalizeStringType<char[len_]> {
    typedef const char* Type;
};
template<std::size_t len_>
struct NormalizeStringType<const char[len_]> {
    typedef const char* Type;
};
template<>
struct NormalizeStringType<char*> {
    typedef const char* Type;
};
template<>
struct NormalizeStringType<const char*> {
    typedef const char* Type;
};

/**
 * @brief A helper trait - get normalized type of a captured assertion parameter
 */
template<typename Type_>
struct AssertionParameter {
    typedef typename NormalizeStringType<
        typename std::remove_const<
            typename std::remove_reference<Type_>::type>::type>::Type Type;
};

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TYPETRAITS_H_ */

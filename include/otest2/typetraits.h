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

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TYPETRAITS_H_ */

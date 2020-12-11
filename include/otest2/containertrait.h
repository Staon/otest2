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

#ifndef OTest2_INCLUDE_OTEST2_CONTAINERTRAIT_H_
#define OTest2_INCLUDE_OTEST2_CONTAINERTRAIT_H_

namespace OTest2 {

/**
 * @brief A simple trait allowing access to list container's iterators
 *
 * This trait is used by the item-wise and lexicographical assertions
 * to get the begin and end iterator. This default implementation works
 * well with the STL containers.
 *
 * One can write own specialization of the the trait to plug own container
 * into the assertions. The iterators must follow:
 *   - the * operator to access the pointed value,
 *   - the prefix ++ operator moving to next item,
 *   - the != operator to to compare iterators.
 */
template<typename Container_>
struct ListContainerTrait {
    /**
     * @brief Get beginning iterator
     */
    static typename Container_::const_iterator begin(
        const Container_& cont_) {
      return cont_.begin();
    }

    /**
     * @brief Get ending iterator
     */
    static typename Container_::const_iterator end(
        const Container_& cont_) {
      return cont_.end();
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CONTAINERTRAIT_H_ */

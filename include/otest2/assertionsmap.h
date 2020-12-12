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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTIONSMAP_H_
#define OTest2_INCLUDE_OTEST2_ASSERTIONSMAP_H_

#include <otest2/assertcontext.h>
#include <otest2/assertionannotation.h>

namespace OTest2 {

/**
 * @brief Implementation of map assertions
 *
 * The map assertions check content of map containers in the item-wise
 * manner. Items are paired by their keys.
 *
 * The default algorithm iterates first and then the second list and checks
 * values in the other list. It's not the most time effective algorithm
 * but it works for ordered maps as well as for unordered maps.
 *
 * The std::multimap are supported too - items with the same key must keep
 * the same order in both containers.
 *
 * As the std::unordered_multimap doesn't keep any defined order of items
 * with the same key, the container is supported only in a special
 * case when there are only unique keys inside.
 *
 * The algorithm is controlled by the MapContainerTrait. The default
 * implementation works with STL-like maps.
 */
class MapAssertion : public AssertContext {
  private:

  public:
    /* -- avoid copying */
    MapAssertion(
        const MapAssertion&) = delete;
    MapAssertion& operator = (
        const MapAssertion&) = delete;

    /* -- inherit the constructor - the parent constructor is invoked
     *    from the test suite generated code. */
    using AssertContext::AssertContext;

    /* -- map assertions */
    template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
    bool testAssertMap(
        const ContainerA_& a_,
        const ContainerB_& b_);
};

namespace Assertions {

/**
 * @brief Compare content of two maps with specified comparator
 *
 * @tparam Compare_ The relation comparator template
 * @param a_ The first container
 * @param b_ The second container
 * @return True if the values fit the comparator
 */
template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool testAssertMap(
    const ContainerA_& a_,
    const ContainerB_& b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::MapAssertion", "testAssertMap< ::$1 >");

} /* -- namespace Assertions */

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTIONSMAP_H_ */

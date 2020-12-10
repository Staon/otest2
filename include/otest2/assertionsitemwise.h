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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTIONSITEMWISE_H_
#define OTest2_INCLUDE_OTEST2_ASSERTIONSITEMWISE_H_

#include <otest2/assertcontext.h>
#include <otest2/assertionannotation.h>

namespace OTest2 {

/**
 * @brief Implementation of item-wise container assertions
 */
class ItemWiseAssertion : public AssertContext {
  private:
    template<
        template<typename, typename> class Compare_,
        typename IterA_,
        typename IterB_>
    bool testAssertItemWiseList(
        IterA_ begin_a_,
        IterA_ end_a_,
        IterB_ begin_b_,
        IterB_ end_b_);

  public:
    /* -- avoid copying */
    ItemWiseAssertion(
        const ItemWiseAssertion&) = delete;
    ItemWiseAssertion& operator = (
        const ItemWiseAssertion&) = delete;

    /* -- inherit the constructor - the parent constructor is invoked
     *    from the test suite generated code. */
    using AssertContext::AssertContext;

    /* -- generic item-wise assertion */
    template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
    bool testAssertItemWise(
        IterA_ begin_a_,
        IterA_ end_a_,
        IterB_ begin_b_,
        IterB_ end_b_);
    template<template<typename, typename> class Compare_, typename ContainerA_, typename IterB_>
    bool testAssertItemWise(
        const ContainerA_& a_,
        IterB_ begin_b_,
        IterB_ end_b_);
    template<template<typename, typename> class Compare_, typename IterA_, typename ContainerB_>
    bool testAssertItemWise(
        IterA_ begin_a,
        IterA_ end_a_,
        const ContainerB_& b_);
    template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
    bool testAssertItemWise(
        const ContainerA_& a_,
        const ContainerB_& b_);
};

/**
 * @brief Itemwise comparison of two lists
 *
 * @tparam Compare_ The comparison operator
 * @param begin_a_ Beginning iterator of the first list
 * @param end_a_ Ending iterator of the first list
 * @param begin_b_ Beginning iterator of the second list
 * @param end_b_ Ending iterator of the second list
 * @return True if the lists are the same
 */
template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
bool testAssertItemWise(
    IterA_ begin_a_,
    IterA_ end_a_,
    IterB_ begin_b_,
    IterB_ end_b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::ItemWiseAssertion", "testAssertItemWise<$1>");

/**
 * @brief Compare two lists
 *
 * @tparam Compare_ The comparison operator
 * @param a_ The first list
 * @param begin_b_ Beginning iterator of the second list
 * @param end_b_ Ending iterator of the second list
 * @return True if the lists are the same
 */
template<template<typename, typename> class Compare_, typename ContainerA_, typename IterB_>
bool testAssertItemWise(
    const ContainerA_& a_,
    IterB_ begin_b_,
    IterB_ end_b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::ItemWiseAssertion", "testAssertItemWise<$1>");

/**
 * @brief Compare two lists
 *
 * @tparam Compare_ The comparison operator
 * @param begin_a_ Beginning iterator of the first list
 * @param end_a_ Ending iterator of the first list
 * @param b_ The second list
 * @return True if the lists are the same
 */
template<template<typename, typename> class Compare_, typename IterA_, typename ContainerB_>
bool testAssertItemWise(
    IterA_ begin_a_,
    IterA_ end_a_,
    const ContainerB_& b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::ItemWiseAssertion", "testAssertItemWise<$1>");

/**
 * @brief Compare two lists
 *
 * @param a_ The first list
 * @param b_ The second list
 * @return True if the lists are the same
 */
template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool testAssertItemWise(
    const ContainerA_& a_,
    const ContainerB_& b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::ItemWiseAssertion", "testAssertItemWise<$1>");

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTIONSITEMWISE_H_ */

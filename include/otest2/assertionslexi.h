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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTIONSLEXI_H_
#define OTest2_INCLUDE_OTEST2_ASSERTIONSLEXI_H_

#include <otest2/assertcontext.h>
#include <otest2/assertionannotation.h>

namespace OTest2 {

/**
 * @brief Implementation of lexicographical container assertions
 *
 * The lexicographical assertions check two lists lexicographically.
 * That means that the algorithm takes one item by one item from the both
 * lists. If the items equal the algorithm moves forward. If the they don't
 * the assertion passes or fails according to used comparator.
 *
 * As the item-wise assertions do the lexicographical assertions use
 * a comparator template. However, these comparators are different than
 * the item-wise ones. They must resolve three relations: less, equal
 * and greater. And the comparator must resolve result for remaining
 * tails of the lists (if one list is a prefix of the other one).
 *
 * Thus the comparator must implements two functions:
 *   * checkItems() which takes one item from the left list and one item
 *     form the right list and returns {-1, 0, 1} in the meaning
 *     {less, equal, greater}.
 *   * checkRests() taking one boolean for the left list and one for the
 *     right list. If the boolean is true the appropriate list has ended
 *     (there is no remaining tail of the list). The function returns true
 *     if the remaining tails fit the operator.
 */
class LexicographicalAssertion : public AssertContext {
  private:
    template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
    bool testAssertLexiList(
        IterA_ begin_a_,
        IterA_ end_a_,
        IterB_ begin_b_,
        IterB_ end_b_);

  public:
    /* -- avoid copying */
    LexicographicalAssertion(
        const LexicographicalAssertion&) = delete;
    LexicographicalAssertion& operator = (
        const LexicographicalAssertion&) = delete;

    /* -- inherit the constructor - the parent constructor is invoked
     *    from the test suite generated code. */
    using AssertContext::AssertContext;

    /* -- lexicographical assertions */
    template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
    bool testAssertLexi(
        IterA_ begin_a_,
        IterA_ end_a_,
        IterB_ begin_b_,
        IterB_ end_b_);
    template<template<typename, typename> class Compare_, typename ContainerA_, typename IterB_>
    bool testAssertLexi(
        const ContainerA_& a_,
        IterB_ begin_b_,
        IterB_ end_b_);
    template<template<typename, typename> class Compare_, typename IterA_, typename ContainerB_>
    bool testAssertLexi(
        IterA_ begin_a_,
        IterA_ end_a_,
        const ContainerB_& b_);
    template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
    bool testAssertLexi(
        const ContainerA_& a_,
        const ContainerB_& b_);
};

/**
 * @brief Lexicographical comparison of two lists
 *
 * @tparam Compare_ A lexicographical comparison operator. Note the lexicographical
 *     comparator is different than item-wise comparators.
 * @param begin_a_ Beginning iterator of the first list
 * @param end_a_ Ending iterator of the first list
 * @param begin_b_ Beginning iterator of the second list
 * @param end_b_ Ending iterator of the second list
 * @return True if the assertion passes
 */
template<template<typename, typename> class Compare_, typename IterA_, typename IterB_>
bool testAssertLexi(
    IterA_ begin_a_,
    IterA_ end_a_,
    IterB_ begin_b_,
    IterB_ end_b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::LexicographicalAssertion", "testAssertLexi< ::$1 >");

/**
 * @brief Lexicographical comparison of two lists
 *
 * @tparam Compare_ A lexicographical comparison operator. Note the lexicographical
 *     comparator is different than item-wise comparators.
 * @param a_ The first list
 * @param begin_b_ Beginning iterator of the second list
 * @param end_b_ Ending iterator of the second list
 * @return True if the assertion passes
 */
template<template<typename, typename> class Compare_, typename ContainerA_, typename IterB_>
bool testAssertLexi(
    const ContainerA_& a_,
    IterB_ begin_b_,
    IterB_ end_b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::LexicographicalAssertion", "testAssertLexi< ::$1 >");

/**
 * @brief Lexicographical comparison of two lists
 *
 * @tparam Compare_ A lexicographical comparison operator. Note the lexicographical
 *     comparator is different than item-wise comparators.
 * @param begin_a_ Beginning iterator of the first list
 * @param end_a_ Ending iterator of the first list
 * @param b_ The second list
 * @return True if the assertion passes
 */
template<template<typename, typename> class Compare_, typename IterA_, typename ContainerB_>
bool testAssertLexi(
    IterA_ begin_a_,
    IterA_ end_a_,
    const ContainerB_& b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::LexicographicalAssertion", "testAssertLexi< ::$1 >");

/**
 * @brief Lexicographical comparison of two lists
 *
 * @tparam Compare_ A lexicographical comparison operator. Note the lexicographical
 *     comparator is different than item-wise comparators.
 * @param a_ The first list
 * @param b_ The second list
 * @return True if the assertion passes
 */
template<template<typename, typename> class Compare_, typename ContainerA_, typename ContainerB_>
bool testAssertLexi(
    const ContainerA_& a_,
    const ContainerB_& b_)
TEST_ASSERTION_MARK_TMPL("::OTest2::LexicographicalAssertion", "testAssertLexi< ::$1 >");

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTIONSLEXI_H_ */

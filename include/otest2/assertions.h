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


#ifndef OTest2__INCLUDE_OTEST2_ASSERTIONS_H_
#define OTest2__INCLUDE_OTEST2_ASSERTIONS_H_

#include <functional>

#include <otest2/assertbean.h>
#include <otest2/assertcontext.h>
#include <otest2/assertionannotation.h>
#include <otest2/comparisons.h>

namespace OTest2 {

/**
 * Implementation notes
 *
 * An assertion is implemented by two items: a declaration and an implementation
 * class. The declaration is a function specifying assertion's parameters.
 * There is no definition of the function - it's needed only by IDEs and
 * the OTest2 parser.
 *
 * The declaration class must inherit from the AssertContext class, it must
 * inherit the parent's constructor and an assertion method with exactly
 * the same parameters as the asssertion's declaration function. The declaration
 * function must be connected to the implementation class and its method by
 * the annotation.
 *
 * The OTest2 preprocessor replaces the invocation of the assertion by
 * instantiation of the implementation class and invocation of the assertion
 * method.
 */

/**
 * @brief Implementation class of the generic assertions
 */
class GenericAssertion : public AssertContext {
  public:
    /* -- avoid copying */
    GenericAssertion(
        const GenericAssertion&) = delete;
    GenericAssertion& operator =(
        const GenericAssertion&) = delete;

    /* -- inherit the constructor - the parent constructor is invoked
     *    from the test suite generated code. */
    using AssertContext::AssertContext;

    /* -- the assertion implementation */
    bool testAssert(
        bool condition_);
    bool testAssert(
        const AssertBean& bean_);
    template<template<typename, typename> class Compare_, typename A_, typename B_>
    bool testAssertCompare(
        A_&& a_,
        B_&& b_);

    /* -- exception assertion - it's never used as an assertion as
     *    it's always generated. */
    bool testException(
        std::function<bool()> ftor_);
};

namespace Assertions {

/**
 * @brief Generic assertion
 *
 * @param condition_ A condition. True means the assertion passes.
 */
bool testAssert(
    bool condition_) TEST_ASSERTION_MARK(::OTest2::GenericAssertion, testAssert);

/**
 * @brief Generic assertion
 *
 * @param bean_ A bean containing assertion's condition and a message.
 *     If the condition is true, the assertion passes.
 */
bool testAssert(
    const AssertBean& bean_) TEST_ASSERTION_MARK(::OTest2::GenericAssertion, testAssert);

/**
 * @brief Generic assertion with parsed expression
 *
 * This assertion function parses specified boolean expression. If the root
 * operator one of the relational operators (==, != etc.) it will separate
 * left and right operands and it will process the assertion as the relational
 * assertion testAssertCmp().
 *
 * If the root operator is not any of the relational operators the assertion
 * works as the generic testAssert() function.
 *
 * @param expression_ The boolean expression
 * @return Value of the boolean expression
 */
bool testAssertExpr(
    bool expression_) TEST_ASSERTION_EXPR_MARK("::OTest2::GenericAssertion", "testAssert", "testAssertCompare< ::$1 >");

/**
 * @brief Generic comparison assertion
 *
 * @tparam Compare_ The comparison template
 * @param a_ Left operand
 * @param b_ Right operand
 * @return Result of the comparison
 */
template<template<typename, typename> class Compare_, typename A_, typename B_>
bool testAssertCmp(
    A_ a_,
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::$1 >");

/**
 * @brief Compare whether @a a_ is equal to @a b_
 *
 * @param a_ Left operand
 * @param b_ Right operand
 * @return Result of the comparison
 */
template<typename A_, typename B_>
bool testAssertEqual(
    A_ a_,
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::Equal >");

/**
 * @brief Compare whether @a a_ is not equal to @a b_
 *
 * @param a_ Left operand
 * @param b_ Right operand
 * @return Result of the comparison
 */
template<typename A_, typename B_>
bool testAssertNotEqual(
    A_ a_,
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::NotEqual >");

/**
 * @brief Compare whether @a a_ is less than @a b_
 *
 * @param a_ Left operand
 * @param b_ Right operand
 * @return Result of the comparison
 */
template<typename A_, typename B_>
bool testAssertLess(
    A_ a_,
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::Less >");

/**
 * @brief Compare whether @a a_ is less than or equal to @a b_
 *
 * @param a_ Left operand
 * @param b_ Right operand
 * @return Result of the comparison
 */
template<typename A_, typename B_>
bool testAssertLessOrEqual(
    A_ a_,
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::LessOrEqual >");

/**
 * @brief Compare whether @a a_ is greater than @a b_
 *
 * @param a_ Left operand
 * @param b_ Right operand
 * @return Result of the comparison
 */
template<typename A_, typename B_>
bool testAssertGreater(
    A_ a_,
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::Greater >");

/**
 * @brief Compare whether @a a_ is greater than or equal to @a b_
 *
 * @param a_ Left operand
 * @param b_ Right operand
 * @return Result of the comparison
 */
template<typename A_, typename B_>
bool testAssertGreaterOrEqual(
    A_ a_,
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::GreaterOrEqual >");

} /* -- namespace Assertions */

} /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTIONS_H_ */

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

#include <otest2/assertbean.h>
#include <otest2/assertcontext.h>
#include <otest2/comparisons.h>

#ifndef OTEST2_PARSER_ACTIVE

#define TEST_ASSERTION_MARK(class_, method_)
#define TEST_ASSERTION_MARK_TMPL(class_, method_)

#else /* -- OTEST2_PARSER_ACTIVE */

#define TEST_ASSERTION_MARK(class_, method_) __attribute__((annotate("otest2::assertion(" #class_ ";" #method_ ")")))
#define TEST_ASSERTION_MARK_TMPL(class_, method_) __attribute__((annotate("otest2::assertion(" class_ ";" method_ ")")))

#endif /* -- OTEST2_PARSER_ACTIVE */

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
        bool condition_) const;
    bool testAssert(
        const AssertBean& bean_) const;
    template<typename Compare_, typename A_, typename B_>
    bool testAssertCompare(
        A_ a_,
        B_ b_) const;
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
 * @brief Comparing of two values
 *
 * The assertion passes if the values are the same.
 *
 * @param a_ Left operand
 * @param b_ Right operand
 * @return Result of the comparison
 */
template<typename Compare_, typename A_, typename B_>
bool testAssertCompare(
    A_ a_,
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< $1<$2, $3> >");

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
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::Equal<$1, $2> >");

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
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::NotEqual<$1, $2> >");

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
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::Less<$1, $2> >");

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
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::LessOrEqual<$1, $2> >");

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
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::Greater<$1, $2> >");

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
    B_ b_) TEST_ASSERTION_MARK_TMPL("::OTest2::GenericAssertion", "testAssertCompare< ::OTest2::GreaterOrEqual<$1, $2> >");

} /* -- namespace Assertions */

} /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTIONS_H_ */

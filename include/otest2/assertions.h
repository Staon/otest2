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

#ifndef OTEST2_PARSER_ACTIVE

#define OTEST2_ANNOT_ASSERTION
#define OTEST2_ANNOT_ASSERTION_EXPR

#else /* -- OTEST2_PARSER_ACTIVE */

#define OTEST2_ANNOT_ASSERTION __attribute__((annotate("otest2::assertion")))
#define OTEST2_ANNOT_ASSERTION_EXPR __attribute__((annotate("otest2::assertionExpr")))

#endif /* -- OTEST2_PARSER_ACTIVE */

namespace OTest2 {

namespace Assertions {

/**
 * @brief Generic assertion
 *
 * @param condition_ A condition. True means the assertion passes.
 */
void testAssert(
    bool condition_) OTEST2_ANNOT_ASSERTION_EXPR;

/**
 * @brief Generic assertion
 *
 * @param bean_ A bean containing assertion's condition and message.
 *     If the condition is true, the assertion passes.
 */
void testAssert(
    const AssertBean& bean_) OTEST2_ANNOT_ASSERTION_EXPR;

/**
 * @brief Comparing of two values
 *
 * The assertion passes if the values are the same.
 *
 * @param expected_ Expected value
 * @param actual_ Actual value
 */
template<typename A_, typename B_>
void testAssertEqual(
    A_ expected_,
    B_ actual_) OTEST2_ANNOT_ASSERTION;

} /* -- namespace Assertions */

} /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTIONS_H_ */

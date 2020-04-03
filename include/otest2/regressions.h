/*
 * Copyright (C) 2020 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_REGRESSIONS_H_
#define OTest2__INCLUDE_OTEST2_REGRESSIONS_H_

#include <string>

#include <otest2/assertcontext.h>
#include <otest2/assertionannotation.h>
#include <otest2/testmarkptr.h>

namespace OTest2 {

/**
 * @brief Implementation class of the regression assertions
 */
class RegressionAssertion : public AssertContext {
  private:
    bool compareObjectMark(
        const std::string& key_,
        TestMarkPtr test_mark_,
        bool print_);
    bool storeObjectMark(
        const std::string& key_,
        TestMarkPtr test_mark_);

  public:
    /* -- avoid copying */
    RegressionAssertion(
        const RegressionAssertion&) = delete;
    RegressionAssertion& operator = (
        const RegressionAssertion&) = delete;

    /* -- inherit the constructor */
    using AssertContext::AssertContext;

    template<typename Object_>
    bool testRegression(
        const std::string& key_,
        const Object_& object_);
    template<typename Object_>
    bool testRegressionP(
        const std::string& key_,
        const Object_& object_);
    template<typename Object_>
    bool testRegressionW(
        const std::string& key_,
        const Object_& object_);
};

namespace Assertions {

/**
 * @brief Compare regression test marks
 *
 * The method compares regression test mark of passed object @a object_
 * and the test mark stored with the same key @a key_. If the marks are
 * the same, the assertion passes.
 *
 * @param key_ key of the regression test mark
 * @param object_ the checked object
 * @return True if the assertion passes
 */
template<typename Object_>
bool testRegression(
    const std::string& key_,
    const Object_& object_) TEST_ASSERTION_MARK_TMPL("::OTest2::RegressionAssertion", "testRegression< $1 >");

/**
 * @brief Compare regression test marks and print current mark
 *
 * The method compares regression test mark of passed object @a object_
 * and the test mark stored with the same key @a key_. If the marks are
 * the same, the assertion passes.
 *
 * @param key_ key of the regression test mark
 * @param object_ the checked object
 * @return True if the assertion passes
 */
template<typename Object_>
bool testRegressionP(
    const std::string& key_,
    const Object_& object_) TEST_ASSERTION_MARK_TMPL("::OTest2::RegressionAssertion", "testRegressionP< $1 >");

/**
 * @brief Store regression test mark
 *
 * This method stores the mark into the storage. The assertion always fails.
 *
 * @param key_ key of the regression test mark
 * @param object_ the checked object
 * @return False
 */
template<typename Object_>
bool testRegressionW(
    const std::string& key_,
    const Object_& object_)TEST_ASSERTION_MARK_TMPL("::OTest2::RegressionAssertion", "testRegressionW< $1 >");

} /* -- namespace Assertions */

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_REGRESSIONS_H_ */

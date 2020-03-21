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

#ifndef OTest2__INCLUDE_OTEST2_REGRESSIONSIMPL_H_
#define OTest2__INCLUDE_OTEST2_REGRESSIONSIMPL_H_

#include <otest2/regressions.h>

#include <otest2/testmarkbuilder.h>

namespace OTest2 {

/**
 * @brief Generic regression trait
 *
 * The generic behavior of the trait is to try to invoke object's method
 * 'test_testMark(TestMarkBuilder&).
 */
template<typename Object_>
struct RegressionTrait {
    static void generateTestMark(
        TestMarkBuilder& builder_,
        const Object_& object_) {
      object_.test_testMark(builder_);
    }
};



template<typename Object_>
bool RegressionAssertion::testRegression(
    const std::string& key_,
    const Object_& object_) {
  /* -- create the mark */
  TestMarkBuilder builder_;
  RegressionTrait<Object_>::generateTestMark(builder_, object_);

  /* -- compare the mark */
  return compareObjectMark(key_, builder_.stealMark());
}

template<typename Object_>
bool RegressionAssertion::testRegressionW(
    const std::string& key_,
    const Object_& object_) {
  /* -- create the mark */
  TestMarkBuilder builder_;
  RegressionTrait<Object_>::generateTestMark(builder_, object_);

  return storeObjectMark(key_, builder_.stealMark());
}

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_REGRESSIONSIMPL_H_ */

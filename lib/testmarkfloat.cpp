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
#include <testmarkfloat.h>

#include <cmath>

namespace OTest2 {

TestMarkFloat::TestMarkFloat(
    long double value_) :
  value(value_) {

}

TestMarkFloat::~TestMarkFloat() {

}

bool TestMarkFloat::doIsEqual(
    const TestMark& other_,
    long double precision_) const {
  return std::abs(value - static_cast<const TestMarkFloat*>(&other_)->value)
      <= precision_;
}

bool TestMarkFloat::doIsEqualValue(
    const TestMark& other_,
    long double precision_) const {
  return doIsEqual(other_, precision_);
}

void TestMarkFloat::doDiffArray(
    const TestMark* parent_,
    const std::string label_,
    std::vector<DiffRecord> array_) const {
  pushDiffMe(parent_, label_, array_);
}

} /* namespace OTest2 */

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

#include <assert.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

#include <otest2/testmarkhash.h>
#include <otest2/testmarkout.h>

namespace OTest2 {

namespace {

const char SERIALIZE_TYPE_MARK[] = "ot2:float";

} /* -- namespace */

TestMarkFloat::TestMarkFloat(
    long double value_) :
  TestMark(TestMarkHash::hashBasicType(SERIALIZE_TYPE_MARK, value_)),
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
    int level_,
    std::vector<LinearizedRecord>& array_) const
{
  /* -- there are no children */
}

void TestMarkFloat::doLinearizedMark(
    int level_,
    const std::string& label_,
    std::vector<LinearizedRecord>& array_) const {
  array_.push_back({level_, this, label_});
}

void TestMarkFloat::doPrintOpen(
    std::ostream& os_,
    const std::string& prefix_) const {
  os_ << prefix_
      << std::setprecision(
          std::numeric_limits<long double>::digits10 + 1)
      << value << '\n';
}

void TestMarkFloat::doPrintClose(
    std::ostream& os_,
    const std::string& prefix_) const {
  /* -- nothing to do */
}

void TestMarkFloat::doSerializeMark(
    TestMarkOut& serializer_) const {
  serializer_.writeTypeMark(SERIALIZE_TYPE_MARK);
  serializer_.writeFloat(value);
}

} /* namespace OTest2 */

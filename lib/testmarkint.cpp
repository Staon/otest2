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
#include <testmarkint.h>

#include <assert.h>
#include <iostream>

#include <otest2/testmarkhash.h>

namespace OTest2 {

TestMarkInt::TestMarkInt(
    int64_t value_) :
  TestMark(TestMarkHash::hashBasicType("int", value_)),
  value(value_) {

}

TestMarkInt::~TestMarkInt() {

}

bool TestMarkInt::doIsEqual(
    const TestMark& other_,
    long double precision_) const {
  return value == static_cast<const TestMarkInt*>(&other_)->value;
}

bool TestMarkInt::doIsEqualValue(
    const TestMark& other_,
    long double precision_) const {
  return doIsEqual(other_, precision_);
}

void TestMarkInt::doDiffArray(
    int level_,
    std::vector<LinearizedRecord>& array_) const
{
  /* -- there are no children */
}

void TestMarkInt::doLinearizedMark(
    int level_,
    const std::string& label_,
    std::vector<LinearizedRecord>& array_) const {
  array_.push_back({level_, this, label_});
}

void TestMarkInt::doPrintOpen(
    std::ostream& os_,
    const std::string& prefix_) const {
  os_ << prefix_ << value << '\n';
}

void TestMarkInt::doPrintClose(
    std::ostream& os_,
    const std::string& prefix_) const {
  /* -- nothing to do */
}

} /* namespace OTest2 */

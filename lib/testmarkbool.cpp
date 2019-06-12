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

#include <otest2/testmarkbool.h>

#include <assert.h>
#include <iostream>

namespace OTest2 {

TestMarkBool::TestMarkBool(
    bool value_) :
  value(value_) {

}

TestMarkBool::~TestMarkBool() {

}

bool TestMarkBool::doIsEqual(
    const TestMark& other_,
    long double precision_) const {
  return value == static_cast<const TestMarkBool*>(&other_)->value;
}

bool TestMarkBool::doIsEqualValue(
    const TestMark& other_,
    long double precision_) const {
  return doIsEqual(other_, precision_);
}

bool TestMarkBool::doIsFirstOrLastChild(
    const TestMark* other_) const {
  assert(false);
  return false;
}

void TestMarkBool::doDiffArray(
    const TestMark* parent_,
    const std::string label_,
    std::vector<DiffRecord>& array_) const {
  pushDiffMe(parent_, label_, array_);
}

void TestMarkBool::doPrintOpen(
    std::ostream& os_,
    const std::string& prefix_) const {
  os_ << prefix_;
  if(value)
    os_ << "true";
  else
    os_ << "false";
  os_ << '\n';
}

void TestMarkBool::doPrintClose(
    std::ostream& os_,
    const std::string& prefix_) const {
  /* -- nothing to do */
}

} /* namespace OTest2 */

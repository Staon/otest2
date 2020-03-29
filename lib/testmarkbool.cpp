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

#include <testmarkbool.h>

#include <assert.h>
#include <iostream>
#include <string>

#include <exctestmarkin.h>
#include <testmarkhash.h>
#include <testmarkin.h>
#include <testmarkout.h>

namespace OTest2 {

namespace {

const char SERIALIZE_TYPE_MARK[] = "ot2:bool";

} /* -- namespace */

TestMarkBool::TestMarkBool(
    bool value_) :
  value(value_) {

}

TestMarkBool::TestMarkBool(
    CtorMark*) :
  value(false) {

}

TestMarkBool::~TestMarkBool() {

}

const char* TestMarkBool::typeMark() {
  return SERIALIZE_TYPE_MARK;
}

TestMarkHashCode TestMarkBool::doGetHashCode() const noexcept {
  return TestMarkHash::hashBasicType(SERIALIZE_TYPE_MARK, value);
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

void TestMarkBool::doDiffArray(
    int level_,
    std::vector<LinearizedRecord>& array_) const
{
  /* -- there are no children */
}

void TestMarkBool::doLinearizedMark(
    int level_,
    const std::string& label_,
    std::vector<LinearizedRecord>& array_) const {
  array_.push_back({level_, this, label_});
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

void TestMarkBool::doSerializeMark(
    TestMarkOut& serializer_) const {
  serializer_.writeTypeMark(SERIALIZE_TYPE_MARK);
  serializer_.writeInt(value ? 1 : 0);
}

void TestMarkBool::doDeserializeMark(
    TestMarkFactory& factory_,
    TestMarkIn& deserializer_) {
  auto value_(deserializer_.readInt());
  switch(value_) {
    case 0:
      value = false;
      break;
    case 1:
      value = true;
      break;
    default:
      throw ExcTestMarkIn("invalid bool value " + std::to_string(value_));
  }
}

} /* namespace OTest2 */

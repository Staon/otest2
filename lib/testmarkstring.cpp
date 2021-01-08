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
#include <testmarkstring.h>

#include <assert.h>
#include <iostream>
#include <utility>

#include <testmarkhash.h>
#include <testmarkin.h>
#include <testmarkout.h>

namespace OTest2 {

namespace {

const char SERIALIZE_TYPE_MARK[] = "ot2:string";

} /* -- namespace */

TestMarkString::TestMarkString(
    const std::string& value_) :
  value(value_) {

}

TestMarkString::TestMarkString(
    const CtorMark*) :
  value() {

}

TestMarkString::~TestMarkString() {

}

const char* TestMarkString::typeMark() {
  return SERIALIZE_TYPE_MARK;
}

TestMarkHashCode TestMarkString::doGetHashCode() const noexcept {
  return TestMarkHash::hashBasicType(SERIALIZE_TYPE_MARK, value);
}

bool TestMarkString::doIsEqual(
    const TestMark& other_,
    long double precision_) const {
  return value == static_cast<const TestMarkString*>(&other_)->value;
}

bool TestMarkString::doIsEqualValue(
    const TestMark& other_,
    long double precision_) const {
  return doIsEqual(other_, precision_);
}

void TestMarkString::doDiffArray(
    int level_,
    std::vector<LinearizedRecord>& array_) const
{
  /* -- there are no children */
}

void TestMarkString::doLinearizedMark(
    int level_,
    const std::string& label_,
    std::vector<LinearizedRecord>& array_) const {
  array_.push_back({level_, this, label_});
}

void TestMarkString::doPrintOpen(
    std::ostream& os_,
    const std::string& prefix_) const {
  os_ << prefix_ << '"' << value << "\"\n";
}

void TestMarkString::doPrintClose(
    std::ostream& os_,
    const std::string& prefix_) const {
  /* -- nothing to do */
}

void TestMarkString::doSerializeMark(
    TestMarkOut& serializer_) const {
  serializer_.writeTypeMark(SERIALIZE_TYPE_MARK);
  serializer_.writeString(value);
}

void TestMarkString::doDeserializeMark(
    TestMarkFactory& factory_,
    TestMarkIn& deserializer_) {
  value = deserializer_.readString();
}

} /* namespace OTest2 */

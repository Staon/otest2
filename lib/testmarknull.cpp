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

#include <testmarknull.h>

#include <assert.h>
#include <iostream>

#include <otest2/testmarkhash.h>
#include <otest2/testmarkout.h>

namespace OTest2 {

namespace {

const char SERIALIZE_TYPE_MARK[] = "ot2:null";

} /* -- namespace */

TestMarkNull::TestMarkNull() {

}

TestMarkNull::TestMarkNull(
    CtorMark*) {

}

TestMarkNull::~TestMarkNull() {

}

const char* TestMarkNull::typeMark() {
  return SERIALIZE_TYPE_MARK;
}

TestMarkHashCode TestMarkNull::doGetHashCode() const noexcept {
  return TestMarkHash::hashBasicType(SERIALIZE_TYPE_MARK, nullptr);
}

bool TestMarkNull::doIsEqual(
    const TestMark& other_,
    long double precision_) const {
  return true;  /* -- the type of the node is enough */
}

bool TestMarkNull::doIsEqualValue(
    const TestMark& other_,
    long double precision_) const {
  return true;  /* -- the type of the node is enough */
}

void TestMarkNull::doDiffArray(
    int level_,
    std::vector<LinearizedRecord>& array_) const
{
  /* -- there are no children */
}

void TestMarkNull::doLinearizedMark(
    int level_,
    const std::string& label_,
    std::vector<LinearizedRecord>& array_) const {
  array_.push_back({level_, this, label_});
}

void TestMarkNull::doPrintOpen(
    std::ostream& os_,
    const std::string& prefix_) const {
  os_ << prefix_ << "<NULL>\n";
}

void TestMarkNull::doPrintClose(
    std::ostream& os_,
    const std::string& prefix_) const {
  /* -- nothing to do */
}

void TestMarkNull::doSerializeMark(
    TestMarkOut& serializer_) const {
  serializer_.writeTypeMark(SERIALIZE_TYPE_MARK);
}

void TestMarkNull::doDeserializeMark(
    TestMarkFactory& factory_,
    TestMarkIn& deserializer_) {
  /* -- nothing to deserialize, the object keeps no data */
}

} /* namespace OTest2 */

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
#include <testmarkprefix.h>

#include <iostream>

#include <testmarkhash.h>
#include <testmarkin.h>
#include <testmarkout.h>

namespace OTest2 {

TestMarkPrefix::TestMarkPrefix(
    const char* serialize_type_mark_,
    const std::string& prefix_) :
  serialize_type_mark(serialize_type_mark_),
  prefix(prefix_),
  hash() {
  hash.addBasicType(serialize_type_mark_, prefix);
}

TestMarkPrefix::TestMarkPrefix(
    const CtorMark*,
    const char* serialize_type_mark_) :
  serialize_type_mark(serialize_type_mark_),
  prefix(),
  hash() {

}

TestMarkPrefix::~TestMarkPrefix() {

}

TestMarkHashCode TestMarkPrefix::doGetHashCode() const noexcept {
  return hash.getHashCode();
}

bool TestMarkPrefix::doIsEqual(
    const TestMark& other_,
    long double precision_) const {
  if(!doIsEqualValue(other_, precision_))
    return false;
  return doIsEqualPrefixed(other_, precision_);
}

bool TestMarkPrefix::doIsEqualValue(
    const TestMark& other_,
    long double precision_) const {
  return prefix == static_cast<const TestMarkPrefix*>(&other_)->prefix;
}

void TestMarkPrefix::doPrintOpen(
    std::ostream& os_,
    const std::string& prefix_) const {
  os_ << prefix_;
  if(!prefix.empty())
    os_ << prefix << ' ';
  const char* pars_(getParenthesis());
  os_ << pars_[0] << '\n';
}

void TestMarkPrefix::doPrintClose(
    std::ostream& os_,
    const std::string& prefix_) const {
  const char* pars_(getParenthesis());
  os_ << prefix_ << pars_[1] << '\n';
}

void TestMarkPrefix::doSerializeMark(
    TestMarkOut& serializer_) const {
  serializer_.writeTypeMark(serialize_type_mark);
  serializer_.writeString(prefix);
  serializeItems(serializer_);
}

void TestMarkPrefix::doDeserializeMark(
    TestMarkFactory& factory_,
    TestMarkIn& deserializer_) {
  prefix = deserializer_.readString();
  hash.addBasicType(serialize_type_mark, prefix);
  deserializeItems(factory_, deserializer_);
}

} /* namespace OTest2 */

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

#include <otest2/testmarkhash.h>

#include <cstring>

namespace OTest2 {

TestMarkHash::TestMarkHash() :
  hash(0xcbf29ce484222325ull) {

}

TestMarkHash::TestMarkHash(
    TestMarkHash&& other_) :
  hash(other_.hash) {

}

TestMarkHash::~TestMarkHash() {

}

void TestMarkHash::addData(
    const std::uint8_t* data_,
    std::size_t size_) {
  std::uint8_t const* const end_(data_ + size_);
  for(; data_ != end_; ++data_) {
    hash *= 0x100000001b3ULL;
    hash ^= static_cast<std::uint64_t>(*data_);
  }
}

void TestMarkHash::addTerminator() {
  std::uint8_t term_(0);
  addData(&term_, sizeof(term_));
}

void TestMarkHash::addHashCode(
    TestMarkHashCode code_) {
  addData(reinterpret_cast<const std::uint8_t*>(&code_), sizeof(code_));
}

void TestMarkHash::addString(
    const char* string_) {
  addData(
      reinterpret_cast<const std::uint8_t*>(string_),
      std::strlen(string_));
}

TestMarkHashCode TestMarkHash::getHashCode() const noexcept {
  return hash;
}

void TestMarkHash::addBasicType(
    const char* type_name_,
    const std::uint8_t* data_,
    std::size_t size_) {
  addString(type_name_);
  addTerminator();
  addData(data_, size_);
}

void TestMarkHash::addBasicType(
    const char* type_name_,
    const std::string& value_) {
  addBasicType(
      type_name_,
      reinterpret_cast<const std::uint8_t*>(value_.c_str()),
      value_.size());

}

TestMarkHashCode TestMarkHash::hashBasicType(
    const char* type_name_,
    const std::string& value_) {
  TestMarkHash hash_;
  hash_.addBasicType(type_name_, value_);
  return hash_.getHashCode();
}

} /* -- namespace OTest2 */

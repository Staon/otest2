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

#include <testmarkoutbin.h>

#include <assert.h>
#include <boost/endian/conversion.hpp>
#include <limits>
#include <cstring>
#include <sstream>
#include <string>

#include "testmarkoutbintags.h"

namespace OTest2 {

TestMarkOutBin::TestMarkOutBin() {

}

TestMarkOutBin::~TestMarkOutBin() {

}

void TestMarkOutBin::writeTag(
    TestMarkOutBinTag tag_) {
  writeBinaryData(sizeof(tag_), reinterpret_cast<const std::uint8_t*>(&tag_));
}

void TestMarkOutBin::writeTypeMark(
    const char* typemark_) {
  assert(typemark_ != nullptr);

  writeTag(TestMarkOutBinTag::TYPE);

  const std::uint64_t length_(std::strlen(typemark_));
  assert(length_ <= std::numeric_limits<std::uint8_t>::max());
  const std::uint8_t mark_len_(static_cast<std::uint8_t>(length_));
  writeBinaryData(sizeof(mark_len_), reinterpret_cast<const std::uint8_t*>(&mark_len_));
  writeBinaryData(length_, reinterpret_cast<const std::uint8_t*>(typemark_));
}

void TestMarkOutBin::writeInt(
    std::int64_t value_) {
  if(value_ >= std::numeric_limits<std::int8_t>::min()
      && value_ <= std::numeric_limits<std::int8_t>::max()) {
    writeTag(TestMarkOutBinTag::INT_SHORT);
    std::uint8_t be_value_(static_cast<std::uint8_t>(static_cast<std::int8_t>(value_)));
    writeBinaryData(sizeof(be_value_), &be_value_);
  }
  else if(value_ >= std::numeric_limits<std::int16_t>::min()
      && value_ <= std::numeric_limits<std::int16_t>::max()) {
    writeTag(TestMarkOutBinTag::INT_NORMAL);
    std::uint16_t be_value_(boost::endian::native_to_big(
        static_cast<std::uint16_t>(static_cast<std::int16_t>(value_))));
    writeBinaryData(sizeof(be_value_), reinterpret_cast<const std::uint8_t*>(&be_value_));
  }
  else {
    writeTag(TestMarkOutBinTag::INT_HUGE);
    std::uint64_t be_value_(
        boost::endian::native_to_big(static_cast<std::uint64_t>(value_)));
    writeBinaryData(sizeof(be_value_), reinterpret_cast<const std::uint8_t*>(&be_value_));
  }
}

void TestMarkOutBin::writeFloat(
    long double value_) {
  writeTag(TestMarkOutBinTag::FLOAT);

  /* -- There is no platform independent way how to store floating points
   *    in binary form. Hence, I print the float in the hexadecimal
   *    format and I store the float as a string. */
  std::ostringstream oss_;
  oss_ << std::hexfloat << value_;
  const std::string& str_value_(oss_.str());

  /* -- write the string */
  const std::uint64_t length_(str_value_.size());
  assert(length_ <= std::numeric_limits<std::uint8_t>::max());
  const std::uint8_t be_length_(static_cast<std::uint8_t>(length_));
  writeBinaryData(sizeof(be_length_), reinterpret_cast<const std::uint8_t*>(&be_length_));
  writeBinaryData(length_, reinterpret_cast<const std::uint8_t*>(str_value_.c_str()));
}

void TestMarkOutBin::writeString(
    const std::string& string_) {
  const std::uint64_t length_(string_.size());

  if(length_ <= std::numeric_limits<std::uint8_t>::max()) {
    writeTag(TestMarkOutBinTag::STRING_SHORT);
    const std::uint8_t be_length_(static_cast<std::uint8_t>(length_));
    writeBinaryData(
        sizeof(be_length_), reinterpret_cast<const std::uint8_t*>(&be_length_));
  }
  else if(length_ <= std::numeric_limits<std::uint16_t>::max()) {
    writeTag(TestMarkOutBinTag::STRING_NORMAL);
    const std::uint16_t be_length_(
        boost::endian::native_to_big(static_cast<std::uint8_t>(length_)));
    writeBinaryData(
        sizeof(be_length_), reinterpret_cast<const std::uint8_t*>(&be_length_));
  }
  else {
    writeTag(TestMarkOutBinTag::STRING_HUGE);
    const std::uint64_t be_length_(boost::endian::native_to_big(length_));
    writeBinaryData(
        sizeof(be_length_), reinterpret_cast<const std::uint8_t*>(&be_length_));
  }
  writeBinaryData(length_, reinterpret_cast<const std::uint8_t*>(string_.c_str()));
}

} /* -- namespace OTest2 */

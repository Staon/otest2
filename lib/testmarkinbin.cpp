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

#include <otest2/testmarkinbin.h>

#include <algorithm>
#include <assert.h>
#include <boost/endian/conversion.hpp>
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <vector>

#include <otest2/exctestmarkin.h>
#include "testmarkoutbintags.h"

#include <iostream>

namespace OTest2 {

TestMarkInBin::TestMarkInBin() {

}

TestMarkInBin::~TestMarkInBin() {

}

TestMarkOutBinTag TestMarkInBin::readTag(
    std::initializer_list<TestMarkOutBinTag> tags_) {
  std::uint8_t rtag_;
  readBinaryData(sizeof(rtag_), &rtag_);
  if(std::find(
      tags_.begin(),
      tags_.end(),
      static_cast<TestMarkOutBinTag>(rtag_)) == tags_.end())
    throw ExcTestMarkIn("unexpected tag in the binary stream");
  return static_cast<TestMarkOutBinTag>(rtag_);
}

std::string TestMarkInBin::readTypeMark() {
  readTag({TestMarkOutBinTag::TYPE});

  std::uint8_t length_;
  readBinaryData(sizeof(length_), &length_);
  boost::endian::big_to_native_inplace(length_);
  std::vector<std::uint8_t> buffer_(length_);
  readBinaryData(length_, buffer_.data());
  return std::string(buffer_.data(), buffer_.data() + length_);
}

std::int64_t TestMarkInBin::readInt() {
  auto tag_(readTag({
      TestMarkOutBinTag::INT_SHORT,
      TestMarkOutBinTag::INT_NORMAL,
      TestMarkOutBinTag::INT_HUGE
  }));

  if(tag_ == TestMarkOutBinTag::INT_SHORT) {
    std::uint8_t buffer_;
    readBinaryData(sizeof(buffer_), &buffer_);
    return static_cast<std::int64_t>(static_cast<std::int8_t>(buffer_));
  }
  else if(tag_ == TestMarkOutBinTag::INT_NORMAL) {
    std::uint16_t buffer_;
    readBinaryData(sizeof(buffer_), reinterpret_cast<std::uint8_t*>(&buffer_));
    boost::endian::big_to_native_inplace(buffer_);
    return static_cast<std::int64_t>(static_cast<std::int16_t>(buffer_));
  }
  else {
    std::uint64_t buffer_;
    readBinaryData(sizeof(buffer_), reinterpret_cast<std::uint8_t*>(&buffer_));
    boost::endian::big_to_native_inplace(buffer_);
    return static_cast<std::int64_t>(buffer_);
  }
}

long double TestMarkInBin::readFloat() {
  readTag({TestMarkOutBinTag::FLOAT});

  std::uint8_t length_;
  readBinaryData(sizeof(length_), &length_);
  std::vector<std::uint8_t> buffer_(length_ + 1);
  readBinaryData(length_, buffer_.data());
  buffer_[length_] = 0;  /* -- string terminator */

  char* end_(nullptr);
  long double value_(
      std::strtold(reinterpret_cast<const char*>(buffer_.data()), &end_));
  if(reinterpret_cast<std::uint8_t*>(end_) != buffer_.data() + length_)
    throw ExcTestMarkIn("invalid format of a floating point number");

  return value_;
}

std::string TestMarkInBin::readString() {
  auto tag_(readTag({
      TestMarkOutBinTag::STRING_SHORT,
      TestMarkOutBinTag::STRING_NORMAL,
      TestMarkOutBinTag::STRING_HUGE
  }));

  std::uint64_t length_;
  if(tag_ == TestMarkOutBinTag::STRING_SHORT) {
    std::uint8_t buffer_;
    readBinaryData(sizeof(buffer_), &buffer_);
    length_ = buffer_;
  }
  else if(tag_ == TestMarkOutBinTag::STRING_NORMAL) {
    std::uint16_t buffer_;
    readBinaryData(sizeof(buffer_), reinterpret_cast<std::uint8_t*>(&buffer_));
    boost::endian::big_to_native_inplace(buffer_);
    length_ = buffer_;
  }
  else {
    std::uint64_t buffer_;
    readBinaryData(sizeof(buffer_), reinterpret_cast<std::uint8_t*>(&buffer_));
    boost::endian::big_to_native_inplace(buffer_);
    length_ = buffer_;
  }
  std::vector<std::uint8_t> str_(length_);
  readBinaryData(length_, str_.data());
  return std::string(str_.data(), str_.data() + length_);
}

} /* -- namespace OTest2 */

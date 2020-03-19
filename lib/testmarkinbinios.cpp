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

#include <otest2/testmarkinbinios.h>

#include <assert.h>
#include <istream>

#include <otest2/exctestmarkin.h>

namespace OTest2 {

TestMarkInBinIOS::TestMarkInBinIOS(
    std::istream* is_) :
  is(is_) {
  assert(is != nullptr);

}

void TestMarkInBinIOS::readBinaryData(
    std::uint64_t size_,
    std::uint8_t* buffer_) {
  is->read(reinterpret_cast<char*>(buffer_), size_);
  if(!*is)
    throw ExcTestMarkIn("an input error");
  if(static_cast<std::uint64_t>(is->gcount()) != size_)
    throw ExcTestMarkIn("missing data in the stream");
}

TestMarkInBinIOS::~TestMarkInBinIOS() {

}

} /* -- namespace OTest2 */

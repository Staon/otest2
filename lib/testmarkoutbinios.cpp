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

#include <otest2/testmarkoutbinios.h>

#include <assert.h>
#include <ostream>

namespace OTest2 {

TestMarkOutBinIOS::TestMarkOutBinIOS(
    std::ostream* os_) :
  os(os_) {
  assert(os != nullptr);

}

TestMarkOutBinIOS::~TestMarkOutBinIOS() {

}

void TestMarkOutBinIOS::writeBinaryData(
    std::uint64_t size_,
    const std::uint8_t* data_) {
  os->write(reinterpret_cast<const char*>(data_), size_);
}

} /* -- namespace OTest2 */

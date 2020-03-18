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

#ifndef OTest2__LIB_TESTMARKOUTBINCONST_H_
#define OTest2__LIB_TESTMARKOUTBINCONST_H_

#include <cstdint>

namespace OTest2 {

enum class TestMarkOutBinTag : std::uint8_t {
  TYPE = 1,
  INT_SHORT = 2,
  INT_NORMAL = 3,
  INT_HUGE = 4,
  FLOAT = 5,
  STRING_SHORT = 6,
  STRING_NORMAL = 7,
  STRING_HUGE = 8,
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__LIB_TESTMARKOUTBINCONST_H_ */

/*
 * Copyright (C) 2021 Ondrej Starek
 *
 * This file is part of OTest2
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

#ifndef OTest2_INCLUDE_OTEST2_REPORTERATTRIBUTES_H_
#define OTest2_INCLUDE_OTEST2_REPORTERATTRIBUTES_H_

#include <cstdint>

namespace OTest2 {

/**
 * @brief List of colors supported by the reporters
 */
enum class Color : std::uint8_t {
  BLACK = 0,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
};

/**
 * @brief Style of shown text
 */
enum class Style : std::uint8_t {
  NORMAL = 0,
  BOLD,
  DIM,
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_REPORTERATTRIBUTES_H_ */

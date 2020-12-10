/*
 * Copyright (C) 2020 Ondrej Starek
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

#ifndef OTest2_INCLUDE_OTEST2_PRINTUTILS_H_
#define OTest2_INCLUDE_OTEST2_PRINTUTILS_H_

#include <iosfwd>

namespace OTest2 {

/**
 * @brief Format the ordinal number with appropriate suffix
 *
 * @param[out] os_ An output stream
 * @param[in] number_ The number
 */
void printOrdinalNumber(
    std::ostream& os_,
    int number_);

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_PRINTUTILS_H_ */

/*
 * Copyright (C) 2018 Ondrej Starek
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

#ifndef OTest2OTEST2_PARSER_H_
#define OTest2OTEST2_PARSER_H_

#include <iosfwd>
#include <string>

namespace OTest2 {

class Generator;

/**
 * @brief Parse a test specification file
 *
 * @param filename_ Name of the input file
 * @param generator_ Generator of the output file
 */
void parse(
    const std::string& filename_);

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_PARSER_H_ */

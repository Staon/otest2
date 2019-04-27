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
#ifndef OTest2__OTEST2_GENERFMT_H_
#define OTest2__OTEST2_GENERFMT_H_

#include <iosfwd>
#include <string>

namespace OTest2 {

/**
 * @brief Write specified string as a C string - in double quotes and escaped
 */
std::ostream& writeCString(
    std::ostream& os_,
    const std::string& text_);

} /* -- namespace OTest2 */

#endif /* OTest2__OTEST2_GENERFMT_H_ */

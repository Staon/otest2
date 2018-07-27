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


#ifndef OTest2__INCLUDE_OTEST2_UTILS_H_
#define OTest2__INCLUDE_OTEST2_UTILS_H_

namespace OTest2 {

/**
 * @brief Delete a pointer and set it invalid
 *
 * @param object_ The pointer
 */
template<typename T_>
void odelete(
    T_*& object_) {
  delete object_;
  object_ = nullptr;
}

} /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_UTILS_H_ */

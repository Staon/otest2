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
#include <testmarkint.h>

namespace OTest2 {

TestMarkInt::TestMarkInt(
    int64_t value_) :
  value(value_) {

}

TestMarkInt::~TestMarkInt() {

}

bool TestMarkInt::doIsEqual(
    const TestMark& other_,
    long double precision_) const {
  return value == static_cast<const TestMarkInt*>(&other_)->value;
}

} /* namespace OTest2 */

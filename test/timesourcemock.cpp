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

#include "timesourcemock.h"

namespace OTest2 {

namespace Test {

TimeSourceMock::TimeSourceMock() :
  just_now(std::chrono::minutes(60 * 27 + 31)) {

}

TimeSourceMock::~TimeSourceMock() {

}

void TimeSourceMock::setTime(
    const TimeSourceMock::time_point& now_) {
  just_now = now_;
}

void TimeSourceMock::addTime(
    const TimeSourceMock::duration& shift_) {
  just_now += shift_;
}

TimeSourceMock::time_point TimeSourceMock::now() {
  return just_now;
}

} /* -- namespace Test */

} /* -- namespace OTest2 */

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

#ifndef OTest2__INCLUDE_OTEST2_TIMESOURCE_H_
#define OTest2__INCLUDE_OTEST2_TIMESOURCE_H_

#include <chrono>

namespace OTest2 {

/**
 * @brief Generic interface for getting of current time
 */
class TimeSource {
  public:
    typedef std::chrono::system_clock::time_point time_point;
    typedef time_point::duration duration;

    /**
     * @brief Ctor
     */
    TimeSource();

    /**
     * @brief Dtor
     */
    virtual ~TimeSource();

    /* -- avoid copying */
    TimeSource(
        const TimeSource&) = delete;
    TimeSource& operator = (
        const TimeSource&) = delete;

    /**
     * @brief Get current time
     */
    virtual time_point now() = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TIMESOURCE_H_ */

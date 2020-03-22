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

#ifndef OTest2__INCLUDE_OTEST2_TIMESOURCESYS_H_
#define OTest2__INCLUDE_OTEST2_TIMESOURCESYS_H_

#include <otest2/timesource.h>

namespace OTest2 {

/**
 * @brief Source of current system time
 */
class TimeSourceSys : public TimeSource {
  public:
    /**
     * brief Ctor
     */
    TimeSourceSys();

    /**
     * @brief Dtor
     */
    virtual ~TimeSourceSys();

    /* -- avoid copying */
    TimeSourceSys(
        const TimeSourceSys&) = delete;
    TimeSourceSys& operator = (
        const TimeSourceSys&) = delete;

    /* -- time source interface */
    virtual time_point now() override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TIMESOURCESYS_H_ */

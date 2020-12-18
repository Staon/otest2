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

#ifndef OTest2__INCLUDE_OTEST2_RUNNERFILTERENTIRE_H_
#define OTest2__INCLUDE_OTEST2_RUNNERFILTERENTIRE_H_

#include <otest2/runnerfilter.h>

namespace OTest2 {

/**
 * @brief Simple runner filter which allows all suites and test cases
 */
class RunnerFilterEntire : public RunnerFilter {
  public:
    /* -- avoid copying */
    RunnerFilterEntire(
        const RunnerFilterEntire&) = delete;
    RunnerFilterEntire& operator =(
        const RunnerFilterEntire&) = delete;

    /**
     * @brief Ctor
     */
    RunnerFilterEntire();

    /**
     * @brief Dtor
     */
    virtual ~RunnerFilterEntire();

    /* -- the runner filter interface */
    virtual bool filterPath(
        const ObjectPath& path_) const noexcept override;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_RUNNERFILTERENTIRE_H_ */

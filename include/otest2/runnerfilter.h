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

#ifndef OTest2__INCLUDE_OTEST2_RUNNERFILTER_H_
#define OTest2__INCLUDE_OTEST2_RUNNERFILTER_H_

#include <string>

namespace OTest2 {

class Registry;
class ObjectPath;

/**
 * @brief Generic interface of a runner filter
 *
 * The runner filter allows filtering of suites and test cases. So the user
 * can specify only some of the test which he wants to run.
 */
class RunnerFilter {
  public:
    /* -- avoid copying */
    RunnerFilter(
        const RunnerFilter&) = delete;
    RunnerFilter& operator =(
        const RunnerFilter&) = delete;

    /**
     * @brief Ctor
     */
    RunnerFilter();

    /**
     * @brief Dtor
     */
    virtual ~RunnerFilter();

    /**
     * @brief Check whether the path should be filtered
     *
     * @param path_ The path
     * @return True if the path matches the filter and the object should be
     *     filtered (not run)
     */
    virtual bool filterPath(
        const ObjectPath& path_) const noexcept = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_RUNNERFILTER_H_ */

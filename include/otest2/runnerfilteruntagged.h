/*
 * Copyright (C) 2021 Ondrej Starek
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

#ifndef OTest2_INCLUDE_OTEST2_RUNNERFILTERUNTAGGED_H_
#define OTest2_INCLUDE_OTEST2_RUNNERFILTERUNTAGGED_H_

#include <otest2/runnerfilter.h>

namespace OTest2 {

/**
 * @brief Run all untagged objects
 */
class RunnerFilterUntagged : public RunnerFilter {
  public:
    /* -- avoid copying */
    RunnerFilterUntagged(
        const RunnerFilterUntagged&) = delete;
    RunnerFilterUntagged& operator = (
        const RunnerFilterUntagged&) = delete;

    /**
     * @brief Ctor
     */
    RunnerFilterUntagged();

    /**
     * @brief Dtor
     */
    virtual ~RunnerFilterUntagged();

    /* -- runner filter */
    virtual bool filterPath(
        const TagsStack& path_) const noexcept override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_RUNNERFILTERUNTAGGED_H_ */

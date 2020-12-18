/*
 * Copyright (C) 2020 Ondrej Starek
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

#ifndef OTest2_INCLUDE_OTEST2_SCENARIOITER_H_
#define OTest2_INCLUDE_OTEST2_SCENARIOITER_H_

#include <otest2/scenarioptr.h>

namespace OTest2 {

/**
 * @brief Generic scenario iterator
 *
 * This interface allows iteration of testing objects in a scenario object
 */
class ScenarioIter {
  public:
    /**
     * @brief Ctor
     */
    ScenarioIter();

    /**
     * @brief Dtor
     */
    virtual ~ScenarioIter();

    /* -- avoid copying */
    ScenarioIter(
        const ScenarioIter&) = delete;
    ScenarioIter& operator = (
        const ScenarioIter&) = delete;

    /**
     * @brief Check whether the iterator points a valid scenario object
     */
    virtual bool isValid() const noexcept = 0;

    /**
     * @brief Get pointed scenario object.
     * @warning The method may be invoked just for a valid iterator!
     */
    virtual ScenarioPtr getScenario() const noexcept = 0;

    /**
     * @brief Move the iterator to next scenario
     */
    virtual void next() noexcept = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_SCENARIOITER_H_ */

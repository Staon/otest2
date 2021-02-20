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

#ifndef OTest2_INCLUDE_OTEST2_SCENARIOCONTAINER_H_
#define OTest2_INCLUDE_OTEST2_SCENARIOCONTAINER_H_

#include <string>

#include <otest2/scenario.h>
#include <otest2/scenariocontainerptr.h>
#include <otest2/scenarioptr.h>

namespace OTest2 {

/**
 * @brief Generic interface of a scenario container
 */
class ScenarioContainer : public Scenario {
  public:
    /**
     * @brief Ctor
     */
    ScenarioContainer();

    /**
     * @brief Dtor
     */
    virtual ~ScenarioContainer();

    /* -- avoid copying */
    ScenarioContainer(
        const ScenarioContainer&) = delete;
    ScenarioContainer& operator = (
        const ScenarioContainer&) = delete;

    /**
     * @brief Append new sub-scenario into the container
     *
     * @param scenario_ The appended scenario
     */
    virtual void appendScenario(
        ScenarioPtr scenario_) = 0;

    /**
     * @brief Check whether the container is empty
     */
    virtual bool isEmpty() const noexcept = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_SCENARIOCONTAINER_H_ */

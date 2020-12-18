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

#ifndef OTest2_INCLUDE_OTEST2_OBJECTSCENARIO_H_
#define OTest2_INCLUDE_OTEST2_OBJECTSCENARIO_H_

#include <otest2/object.h>

#include <otest2/objectptr.h>
#include <otest2/objectscenarioptr.h>
#include <otest2/scenarioptr.h>

namespace OTest2 {

class Context;

/**
 * @brief Inteface of a testing object scheduled by the scenario (test,
 *     suite and test case)
 */
class ObjectScenario : public Object {
  public:
    /**
     * @brief Dtor
     */
    ObjectScenario();

    /**
     * @brief Dtor
     */
    virtual ~ObjectScenario();

    /* -- avoid copying */
    ObjectScenario(
        const ObjectScenario&) = delete;
    ObjectScenario& operator = (
        const ObjectScenario&) = delete;

    /**
     * @brief Execute start-up function at specified index
     *
     * @param context_ The OTest2 context
     * @param index_ The zero based index of the start-up function
     * @return True if the start-up function exists and has been run
     */
    virtual bool startUpObject(
        const Context& context_,
        int index_) = 0;

    /**
     * @brief Schedule body of the testing object
     *
     * @param context_ The OTest2 context
     * @param scenario_ The scenario representing this object
     * @param me_ A shared pointer pointing this instance
     */
    virtual void scheduleBody(
        const Context& context_,
        ScenarioPtr scenario_,
        ObjectPtr me_) = 0;

    /**
     * @brief Execute tear-down function at specified index
     *
     * @param context_ The OTest2 context
     * @param index_ The zero based index of the tear-down function
     */
    virtual void tearDownObject(
        const Context& context_,
        int index_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_OBJECTSCENARIO_H_ */

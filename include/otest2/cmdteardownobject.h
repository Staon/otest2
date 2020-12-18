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

#ifndef OTest2_INCLUDE_OTEST2_CMDTEARDOWNOBJECT_H_
#define OTest2_INCLUDE_OTEST2_CMDTEARDOWNOBJECT_H_

#include <otest2/command.h>
#include <otest2/objectscenarioptr.h>
#include <otest2/scenarioptr.h>

namespace OTest2 {

/**
 * @brief Run a tear-down function
 */
class CmdTearDownObject : public Command {
  private:
    ObjectScenarioPtr object;
    ScenarioPtr scenario;
    int index;

  public:
    /**
     * @brief Ctor
     *
     * @param object_ The testing object
     * @param scenario_ The scenario representing the testing object
     * @param index_ Zero base index of the tear-down function
     */
    explicit CmdTearDownObject(
        ObjectScenarioPtr object_,
        ScenarioPtr scenario_,
        int index_);

    /**
     * @brief Dtor
     */
    virtual ~CmdTearDownObject();

    /* -- avoid copying */
    CmdTearDownObject(
        const CmdTearDownObject&) = delete;
    CmdTearDownObject& operator = (
        const CmdTearDownObject&) = delete;

    /* -- command interface */
    virtual void run(
        const Context& context_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CMDTEARDOWNOBJECT_H_ */

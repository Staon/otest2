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

#ifndef OTest2_INCLUDE_OTEST2_CMDNEXTOBJECT_H_
#define OTest2_INCLUDE_OTEST2_CMDNEXTOBJECT_H_

#include <otest2/command.h>
#include <otest2/objectptr.h>
#include <otest2/scenarioiterptr.h>

namespace OTest2 {

/**
 * @brief Schedule run of next testing object
 */
class CmdNextObject : public Command {
  private:
    ScenarioIterPtr scenario_iter;
    ObjectPtr parent;

  public:
    /**
     * @brief Ctor
     *
     * @param scenario_iter_ An iterator of current level of the scenario
     * @param parent_ Parent object of the testing object. It may be null
     *     for root object.
     */
    explicit CmdNextObject(
        ScenarioIterPtr scenario_iter_,
        ObjectPtr parent_);

    /**
     * @brief Dtor
     */
    virtual ~CmdNextObject();

    /* -- avoid copying */
    CmdNextObject(
        const CmdNextObject&) = delete;
    CmdNextObject& operator = (
        const CmdNextObject&) = delete;

    /* -- command interface */
    virtual void run(
        const Context& context_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CMDNEXTOBJECT_H_ */

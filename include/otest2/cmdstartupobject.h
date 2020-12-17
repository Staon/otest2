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

#ifndef OTest2_INCLUDE_OTEST2_CMDSTARTUPOBJECT_H_
#define OTest2_INCLUDE_OTEST2_CMDSTARTUPOBJECT_H_

#include <otest2/command.h>
#include <otest2/objectptr.h>
#include <otest2/objectscenarioptr.h>
#include <otest2/scenarioptr.h>

namespace OTest2 {

/**
 * @brief Start a testing object up
 */
class CmdStartUpObject : public Command {
  private:
    ObjectScenarioPtr object;
    ScenarioPtr scenario;
    ObjectPtr parent;
    int index;

  public:
    /**
     * @brief Ctor
     *
     * @param object_ The starting testing object
     * @param scenario_ The scenario representing the object
     * @param parent_ Parent of the testing object. It may be null for
     *     a root object
     * @param index_ Zero based index of the start-up method
     */
    explicit CmdStartUpObject(
        ObjectScenarioPtr object_,
        ScenarioPtr scenario_,
        ObjectPtr parent_,
        int index_);

    /**
     * @brief Dtor
     */
    virtual ~CmdStartUpObject();

    /* -- avoid copying */
    CmdStartUpObject(
        const CmdStartUpObject&) = delete;
    CmdStartUpObject& operator = (
        const CmdStartUpObject&) = delete;

    /* -- command interface */
    virtual void run(
        const Context& context_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CMDSTARTUPOBJECT_H_ */

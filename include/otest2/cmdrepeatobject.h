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

#ifndef OTest2_INCLUDE_OTEST2_CMDREPEATOBJECT_H_
#define OTest2_INCLUDE_OTEST2_CMDREPEATOBJECT_H_

#include <string>

#include <otest2/command.h>
#include <otest2/objectptr.h>
#include <otest2/objectrepeaterptr.h>
#include <otest2/scenarioptr.h>

namespace OTest2 {

/**
 * @brief Schedule repeated run of testing object
 */
class CmdRepeatObject : public Command {
  private:
    ScenarioPtr scenario;
    ObjectRepeaterPtr repeater;
    std::string name;
    ObjectPtr parent;

  public:
    /**
     * @brief Ctor
     *
     * @param scenario_ Scenario object representing the testing object
     * @param repeater_ The repeater
     * @param name_ Name of the repeated testing object
     * @param parent_ Parent object of the testing object
     */
    explicit CmdRepeatObject(
        ScenarioPtr scenarion_,
        ObjectRepeaterPtr repeater_,
        const std::string& name_,
        ObjectPtr parent_);

    /**
     * @brief Dtor
     */
    virtual ~CmdRepeatObject();

    /* -- avoid copying */
    CmdRepeatObject(
        const CmdRepeatObject&) = delete;
    CmdRepeatObject& operator = (
        const CmdRepeatObject&) = delete;

    /* -- command interface */
    virtual void run(
        const Context& context_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CMDREPEATOBJECT_H_ */

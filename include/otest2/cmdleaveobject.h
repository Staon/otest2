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

#ifndef OTest2_INCLUDE_OTEST2_CMDLEAVEOBJECT_H_
#define OTest2_INCLUDE_OTEST2_CMDLEAVEOBJECT_H_

#include <string>

#include <otest2/command.h>
#include <otest2/scenarioptr.h>

namespace OTest2 {

/**
 * @brief Leaving of a testing object
 */
class CmdLeaveObject : public Command {
  private:
    ScenarioPtr scenario;
    std::string decorated_name;

  public:
    /**
     * @brief Ctor
     *
     * @param scenario_ The scenario object representing the testing object
     * @param decorated_name_ Decorated name of the testing object
     */
    explicit CmdLeaveObject(
        ScenarioPtr scenario_,
        const std::string& decorated_name_);

    /**
     * @brief Dtor
     */
    virtual ~CmdLeaveObject();

    /* -- avoid copying */
    CmdLeaveObject(
        const CmdLeaveObject&) = delete;
    CmdLeaveObject& operator = (
        const CmdLeaveObject&) = delete;

    /* -- command interface */
    virtual void run(
        const Context& context_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CMDLEAVEOBJECT_H_ */

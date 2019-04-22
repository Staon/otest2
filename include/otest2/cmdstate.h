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

#ifndef OTest2__INCLUDE_OTEST2_CMDSTATE_H_
#define OTest2__INCLUDE_OTEST2_CMDSTATE_H_

#include <string>

#include <otest2/caseordinaryptr.h>
#include <otest2/command.h>

namespace OTest2 {

class CmdState : public Command {
  private:
    CaseOrdinaryPtr testcase;
    std::string name;
    int delay;

  public:
    /* -- avoid copying */
    CmdState(
        const CmdState&) = delete;
    CmdState& operator =(
        const CmdState&) = delete;

    /**
     * @brief Ctor
     *
     * @param testcase_ State's parent
     * @param name_ Name of the state
     * @param delay_ Delay of the command in milliseconds
     */
    explicit CmdState(
        CaseOrdinaryPtr testcase_,
        const std::string& name_,
        int delay_);

    /**
     * @brief Dtor
     */
    virtual ~CmdState();

    /* -- command interface */
    virtual void run(
        const Context& context_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CMDSTATE_H_ */

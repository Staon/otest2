/*
 * Copyright (C) 2018 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_CMDRUNSTATE_H_
#define OTest2__INCLUDE_OTEST2_CMDRUNSTATE_H_

#include <otest2/command.h>
#include <otest2/stateordinaryptr.h>

namespace OTest2 {

/**
 * @brief Run a state
 */
class CmdRunState : public Command {
  private:
    StateOrdinaryPtr state;
    bool wait_before;
    int delay;

  public:
    /* -- avoid copying */
    CmdRunState(
        const CmdRunState&) = delete;
    CmdRunState& operator =(
        const CmdRunState&) = delete;

    /**
     * @brief Ctor
     *
     * @param state_ The state being run
     * @param wait_before_ If it's true the delay is applied.
     * @param delay_ Delay before run of the state in milliseconds.
     */
    explicit CmdRunState(
        StateOrdinaryPtr state_,
        bool wait_before_,
        int delay_);

    /**
     * @brief Dtor
     */
    virtual ~CmdRunState();

    /* -- command interface */
    virtual bool shouldWait(
        const Context& context_,
        int& delay_);
    virtual void run(
        const Context& context_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CMDRUNSTATE_H_ */

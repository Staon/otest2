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

#ifndef OTest2INCLUDE_STATE_H_
#define OTest2INCLUDE_STATE_H_

#include <otest2/caseordinaryptr.h>
#include <otest2/object.h>
#include <otest2/stateptr.h>

namespace OTest2 {

class Context;

/**
 * @brief Generic test state interface
 */
class State : public Object {
  private:
    /* -- avoid copying */
    State(
        const State&);
    State& operator =(
        const State&);

  public:
    /**
     * @brief Ctor
     */
    State();

    /**
     * @brief Dtor
     */
    virtual ~State();

    /**
     * @brief Schedule run of the state
     *
     * @param context_ The otest2 context
     * @param parent_ A smart pointer pointing the parent of the state
     * @param this_ptr_ A smart pointer which keeps lifetime of this state
     * @param delay_cmd_ If it's true, the run of the command is delayed
     * @param delay_ Delay of the command in milliseconds. The value is meant
     *     only if the @a wait_ is true.
     */
    virtual void scheduleRun(
        const Context& context_,
        CaseOrdinaryPtr parent_,
        StatePtr this_ptr_,
        bool delay_cmd_,
        int delay_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_STATE_H_ */

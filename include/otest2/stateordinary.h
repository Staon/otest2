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

#ifndef OTest2__INCLUDE_OTEST2_STATEORDINARY_H_
#define OTest2__INCLUDE_OTEST2_STATEORDINARY_H_

#include <otest2/caseordinaryptr.h>
#include <otest2/state.h>

namespace OTest2 {

class Context;

/**
 * @brief Ordinary state class
 */
class StateOrdinary : public State {
  public:
    /* -- avoid copying */
    StateOrdinary(
        const StateOrdinary&) = delete;
    StateOrdinary& operator =(
        const StateOrdinary&) = delete;

    /**
     * @brief Ctor
     *
     * @param context_ The OTest2 context
     */
    explicit StateOrdinary(
        const Context& context_);

    /**
     * @brief Dtor
     */
    virtual ~StateOrdinary();

    /**
     * @brief Run the state
     *
     * @param context_ The OTest2 context
     * @param parent_ State's parent object
     */
    virtual void executeState(
        const Context& context_,
        CaseOrdinaryPtr parent_) = 0;

    /* -- state interface */
    virtual void scheduleRun(
        const Context& context_,
        CaseOrdinaryPtr parent_,
        StatePtr this_ptr_,
        bool wait_,
        int delay_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_STATEORDINARY_H_ */

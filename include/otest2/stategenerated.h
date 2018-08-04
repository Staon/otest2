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

#ifndef OTest2INCLUDE_STATEGENERATED_H_
#define OTest2INCLUDE_STATEGENERATED_H_

#include <string>

#include <otest2/contextobject.h>
#include <otest2/stateordinary.h>

namespace OTest2 {

class Context;

/**
 * @brief Common implementation of generated states
 */
class StateGenerated : public StateOrdinary, public ContextObject {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    StateGenerated(
        const StateGenerated&) = delete;
    StateGenerated& operator =(
        const StateGenerated&) = delete;

    /**
     * @brief Ctor
     *
     * @param context_ OTest2 context
     * @param name_ Name of the state
     */
    explicit StateGenerated(
        const Context& context_,
        const std::string& name_);

    /**
     * @brief Dtor
     */
    virtual ~StateGenerated();

    /* -- object's interface */
    virtual std::string getName() const;

    /* -- ordinary state interface */
    virtual void executeState(
        const Context& context_);

    /* -- context object */
    virtual const Context& otest2Context() const;

  private:
    /**
     * @brief The user code
     */
    virtual void runState() = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_STATEGENERATED_H_ */

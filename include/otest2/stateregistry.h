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

#ifndef OTest2__INCLUDE_OTEST2_STATEREGISTRY_H_
#define OTest2__INCLUDE_OTEST2_STATEREGISTRY_H_

#include <string>

#include <otest2/stateptr.h>

namespace OTest2 {

/**
 * @brief Registry of test states
 */
class StateRegistry {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    StateRegistry(
        const StateRegistry&) = delete;
    StateRegistry& operator =(
        const StateRegistry&) = delete;

    /**
     * @brief Ctor
     */
    StateRegistry();

    /**
     * @brief Dtor
     */
    ~StateRegistry();

    /**
     * @brief Register new state
     *
     * @param name_ Name of the state
     * @param state_ The state
     */
    void registerState(
        const std::string& name_,
        StatePtr state_);

    /**
     * @brief Get the initial state
     *
     * @return The state or the nullptr if the registry is empty.
     */
    StatePtr getFirstState() const;

    /**
     * @brief Get state with specified @a name_
     *
     * @param name_ Name of the state
     * @return The state or null pointer
     */
    StatePtr getState(
        const std::string& name_) const;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_STATEREGISTRY_H_ */

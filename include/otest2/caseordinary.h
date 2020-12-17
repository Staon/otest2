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

#ifndef OTest2__INCLUDE_OTEST2_CASEORDINARY_H_
#define OTest2__INCLUDE_OTEST2_CASEORDINARY_H_

#include <string>

#include <otest2/case.h>
#include <otest2/caseordinaryptr.h>
#include <otest2/stateptr.h>

namespace OTest2 {

class Context;

/**
 * @brief An ordinary case object scheduled by the CmdFirstState,
 *     CmdRunState and CmdState commands.
 */
class CaseOrdinary : public Case {
  public:
    /* -- avoid copying */
    CaseOrdinary(
        const CaseOrdinary&) = delete;
    CaseOrdinary& operator =(
        const CaseOrdinary&) = delete;

    /**
     * @brief Ctor
     *
     * @param context_ The otest2 context
     */
    explicit CaseOrdinary(
        const Context& context_);

    /**
     * @brief Dtor
     */
    virtual ~CaseOrdinary();

    /**
     * @brief Get the initial state
     *
     * @return The state or nullptr if the case doesn't contain any one.
     */
    virtual StatePtr getFirstState() const = 0;

    /**
     * @brief Get state with specified @a name_
     *
     * @param name_ Name of the state
     * @return The state or null pointer
     */
    virtual StatePtr getState(
        const std::string& name_) const = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CASEORDINARY_H_ */

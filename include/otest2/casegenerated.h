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

#ifndef OTest2INCLUDE_CASEGENERATED_H_
#define OTest2INCLUDE_CASEGENERATED_H_

#include <string>

#include <otest2/caseordinary.h>
#include <otest2/stateptr.h>

namespace OTest2 {

class Context;

/**
 * @brief Common implementation of a generated test case
 */
class CaseGenerated : public CaseOrdinary {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    CaseGenerated(
        const CaseGenerated&) = delete;
    CaseGenerated& operator =(
        const CaseGenerated&) = delete;

    /**
     * @brief Ctor
     *
     * @param context_ OTest2 context
     * @param name_ Name of the test case
     */
    explicit CaseGenerated(
        const Context& context_,
        const std::string& name_);

    /**
     * @brief Dtor
     */
    virtual ~CaseGenerated();

    /* -- object interface */
    virtual std::string getName() const;

    /* -- case ordinary interface */
    virtual StatePtr getFirstState() const;
    virtual void startUpCase(
        const Context& context_);
    virtual void tearDownCase(
        const Context& context_);

  protected:
    /**
     * @brief Register new test state
     *
     * @param name_ Name of the state
     * @param state_ The state
     */
    void registerState(
        const std::string& name_,
        StatePtr state_);

  private:
    /**
     * @brief User start-up code
     */
    virtual void startUp() = 0;

    /**
     * @brief User clean-up code
     */
    virtual void tearDown() = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_CASEGENERATED_H_ */

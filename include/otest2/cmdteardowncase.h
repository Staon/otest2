/*
 * Copyright (C) 2020 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_CMDTEARDOWNCASE_H_
#define OTest2__INCLUDE_OTEST2_CMDTEARDOWNCASE_H_

#include <otest2/caseordinaryptr.h>
#include <otest2/command.h>

namespace OTest2 {

/**
 * @brief Run a tear-down function of a test-case
 */
class CmdTearDownCase : public Command {
  private:
    CaseOrdinaryPtr testcase;
    int index;

  public:
    /**
     * @brief Ctor
     *
     * @param testcase_ The test case
     * @param index_ A zero based index of the tear-down function
     */
    explicit CmdTearDownCase(
        CaseOrdinaryPtr testcase_,
        int index_);

    /**
     * @brief Dtor
     */
    virtual ~CmdTearDownCase();

    /* -- avoid copying */
    CmdTearDownCase(
        const CmdTearDownCase&) = delete;
    CmdTearDownCase& operator = (
        const CmdTearDownCase&) = delete;

    /* -- command interface */
    virtual void run(
        const Context& context_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_CMDTEARDOWNCASE_H_ */

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

#ifndef OTest2__INCLUDE_OTEST2_CMDSTOPSUITE_H_
#define OTest2__INCLUDE_OTEST2_CMDSTOPSUITE_H_

#include <otest2/command.h>
#include <otest2/suiteordinaryptr.h>

namespace OTest2 {

/**
 * @brief Stop a running suite
 */
class CmdStopSuite : public Command {
  private:
    SuiteOrdinaryPtr suite;

  public:
    /* -- avoid copying */
    CmdStopSuite(
        const CmdStopSuite&) = delete;
    CmdStopSuite& operator =(
        const CmdStopSuite&) = delete;

    /**
     * @brief Ctor
     *
     * @param suite_ The ending suite
     */
    explicit CmdStopSuite(
        SuiteOrdinaryPtr suite_);

    /**
     * @brief Dtor
     */
    virtual ~CmdStopSuite();

    /* -- command interface */
    virtual void run(
        const Context& context_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CMDSTOPSUITE_H_ */

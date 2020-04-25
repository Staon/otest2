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

#ifndef OTest2__INCLUDE_OTEST2_CMDSTARTUPSUITE_H_
#define OTest2__INCLUDE_OTEST2_CMDSTARTUPSUITE_H_

#include <otest2/command.h>
#include <otest2/suiteordinaryptr.h>

namespace OTest2 {

class Context;

/**
 * @brief A command which runs start-up functions of a suite
 */
class CmdStartUpSuite : public Command {
  private:
    SuiteOrdinaryPtr suite;
    int index;

  public:
    /**
     * @brief Ctor
     *
     * @param suite_ The suite
     * @param index_ A zero based index of the start-up function
     */
    explicit CmdStartUpSuite(
        SuiteOrdinaryPtr suite_,
        int index_);

    /**
     * @brief Dtor
     */
    virtual ~CmdStartUpSuite();

    /* -- avoid copying */
    CmdStartUpSuite(
        const CmdStartUpSuite&) = delete;
    CmdStartUpSuite& operator = (
        const CmdStartUpSuite&) = delete;

    /* -- command interface */
    virtual void run(
        const Context& context_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_CMDSTARTUPSUITE_H_ */

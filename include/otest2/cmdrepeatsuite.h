/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2
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

#ifndef OTest2_INCLUDE_OTEST2_CMDREPEATSUITE_H_
#define OTest2_INCLUDE_OTEST2_CMDREPEATSUITE_H_

#include <otest2/command.h>
#include <otest2/suiterepeaterptr.h>
#include <string>

namespace OTest2 {

/**
 * @brief A suite repeating command
 *
 * This command runs a suite several times according the SuiteRepeater interface
 */
class CmdRepeatSuite : public Command {
  private:
    SuiteRepeaterPtr repeater;
    std::string suite_name;

  public:
    /* -- avoid copying */
    CmdRepeatSuite(
        const CmdRepeatSuite&) = delete;
    CmdRepeatSuite& operator = (
        const CmdRepeatSuite&) = delete;

    /**
     * @brief Ctor
     *
     * @param repeater_ The repeater object
     * @param suite_name_ Bare (not decorated) suite name
     */
    explicit CmdRepeatSuite(
        SuiteRepeaterPtr repeater_,
        const std::string& suite_name_);

    /**
     * @brief Dtor
     */
    virtual ~CmdRepeatSuite();

    /* -- command interface */
    virtual void run(
        const Context& context_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CMDREPEATSUITE_H_ */

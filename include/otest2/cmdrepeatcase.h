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

#ifndef OTest2_INCLUDE_OTEST2_CMDREPEATCASE_H_
#define OTest2_INCLUDE_OTEST2_CMDREPEATCASE_H_

#include <otest2/caserepeaterptr.h>
#include <otest2/command.h>

namespace OTest2 {

/**
 * @brief Repeated run of a test case
 */
class CmdRepeatCase : public Command {
  private:
    CaseRepeaterPtr repeater;
    std::string suite_name;
    std::string case_name;

  public:
    /* -- avoid copying */
    CmdRepeatCase(
        const CmdRepeatCase&) = delete;
    CmdRepeatCase& operator = (
        const CmdRepeatCase&) = delete;

    /**
     * @brief Ctor
     *
     * @param repeater_ The case repeater object
     * @param suite_name_ Name of the parent suite
     * @param case_name_ Bare name of the testcase
     */
    explicit CmdRepeatCase(
        CaseRepeaterPtr repeater_,
        const std::string& suite_name_,
        const std::string& case_name_);

    /**
     * @brief Dtor
     */
    virtual ~CmdRepeatCase();

    /* -- command interface */
    virtual void run(
        const Context& context_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CMDREPEATCASE_H_ */

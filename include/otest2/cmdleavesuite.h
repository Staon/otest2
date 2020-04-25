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

#ifndef OTest2__INCLUDE_OTEST2_CMDLEAVESUITE_H_
#define OTest2__INCLUDE_OTEST2_CMDLEAVESUITE_H_

#include <otest2/command.h>

namespace OTest2 {

/**
 * @brief Finish suite's stack frame
 */
class CmdLeaveSuite : public Command {
  public:
    /* -- avoid copying */
    CmdLeaveSuite(
        const CmdLeaveSuite&) = delete;
    CmdLeaveSuite& operator =(
        const CmdLeaveSuite&) = delete;

    /**
     * @brief Ctor
     */
    CmdLeaveSuite();

    /**
     * @brief Dtor
     */
    virtual ~CmdLeaveSuite();

    /* -- command interface */
    virtual void run(
        const Context& context_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CMDLEAVESUITE_H_ */
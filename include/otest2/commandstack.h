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

#ifndef OTest2__INCLUDE_OTEST2_COMMANDSTACK_H_
#define OTest2__INCLUDE_OTEST2_COMMANDSTACK_H_

#include <otest2/commandptr.h>

namespace OTest2 {

/**
 * @brief Stack of framework commands
 */
class CommandStack {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    CommandStack(
        const CommandStack&) = delete;
    CommandStack& operator =(
        const CommandStack&) = delete;

    /**
     * @brief Ctor
     */
    CommandStack();

    /**
     * @brief Dtor
     */
    ~CommandStack();

    /**
     * @brief Push a command into the stack
     *
     * @param command_ The command
     */
    void pushCommand(
        CommandPtr command_);

    /**
     * @brief Get the top command
     *
     * @return The command
     */
    CommandPtr topCommand() const;

    /**
     * @brief Pop a command from the top of the stack
     */
    void popCommand();

    /**
     * @brief Check whether the stack is empty
     */
    bool empty() const;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_COMMANDSTACK_H_ */

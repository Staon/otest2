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

#ifndef OTest2__INCLUDE_OTEST2_COMMAND_H_
#define OTest2__INCLUDE_OTEST2_COMMAND_H_

namespace OTest2 {

class Context;

/**
 * @brief generic command interface
 */
class Command {
  public:
    /* -- avoid copying */
    Command(
        const Command&) = delete;
    Command& operator =(
        const Command&) = delete;

    /**
     * @brief Ctor
     */
    Command();

    /**
     * @brief Dtor
     */
    virtual ~Command();

    /**
     * @brief Say whether the framework should wait before running the command
     *
     * @param[in] context_  the OTest2 context
     * @param[out] delay_ The waiting delay in milliseconds. It's valid only
     *     if the true value is returned.
     * @return True forces the waiting.
     * @note The default behavior is not wait.
     */
    virtual bool shouldWait(
        const Context& context_,
        int& delay_);

    /**
     * @brief Run the command
     *
     * @param context_ The OTest2 context
     */
    virtual void run(
        const Context& context_) = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_COMMAND_H_ */

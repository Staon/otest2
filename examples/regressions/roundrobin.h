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

#ifndef OTest2__EXAMPLES_REGRESSIONS_ROUNDROBIN_H_
#define OTest2__EXAMPLES_REGRESSIONS_ROUNDROBIN_H_

#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace OTest2 {

class TestMarkBuilder;

namespace Examples {

/**
 * @brief Generic interface of the round-robin command
 */
class Command {
  public:
    /**
     * @brief Ctor
     */
    Command();

    /**
     * @brief Dtor
     */
    virtual ~Command();

    /* -- avoid copying */
    Command(
        const Command&) = delete;
    Command& operator = (
        const Command&) = delete;

    /**
     * @brief Get name of the command
     */
    virtual std::string getName() const noexcept = 0;

    /**
     * @brief Execute the command
     *
     * @return True if the command has finished.
     */
    virtual bool execute() = 0;
};

/**
 * @brief Round-robin scheduler with priorities
 */
class RoundRobin {
  private:
    typedef std::deque<std::shared_ptr<Command>> Queue;
    typedef std::map<int, Queue> Scheduler;
    Scheduler scheduler;

  public:
    /**
     * @brief Ctor
     */
    RoundRobin();

    /**
     * @brief Dtor
     */
    ~RoundRobin();

    /**
     * @brief Add new command
     *
     * @param priority_ Priority - lesser number means higher priority
     * @param command_ The command
     */
    void scheduleCommand(
        int priority_,
        std::shared_ptr<Command> command_);

    /**
     * @brief Execute next scheduled command
     */
    void executeNextCommand();

    /**
     * @brief Testing method: construction of the regression mark
     */
    void test_testMark(
        TestMarkBuilder& builder_) const noexcept;
};

} /* -- namespace Examples */

} /* -- namespace OTest2 */

#endif /* -- OTest2__EXAMPLES_REGRESSIONS_ROUNDROBIN_H_ */

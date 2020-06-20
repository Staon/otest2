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

#ifndef OTest2_INCLUDE_OTEST2_REPEATER_H_
#define OTest2_INCLUDE_OTEST2_REPEATER_H_

#include <string>

namespace OTest2 {

class Context;

/**
 * @brief Generic repeater interface
 *
 * This class is used as a parent of user data controlling repeating
 * of the test object.
 *
 * The repeater interface has just a boolean getter checking whether next
 * run is prepared.
 *
 * The actual implementation must implement a static method:
 *
 *     static std::shared_ptr<ActuralRepeaterClass> prepareNextRun(
 *         const Context& context_,
 *         std::shared_ptr<ActuralRepeaterClass> current_,
 *         ... other parameters ...);
 *
 * The method returns new or recycled instance of the repeater object or
 * null pointer if there is no next expected run.
 */
class Repeater {
  public:
    /* -- avoid copying */
    Repeater(
        const Repeater&) = delete;
    Repeater& operator = (
        const Repeater&) = delete;

    /**
     * @brief Ctor
     */
    Repeater();

    /**
     * @brief Dtor
     */
    virtual ~Repeater();

    /**
     * @brief Check whether there is a next run prepared
     *
     * @param context_ OTest2 context
     * @return true if there is next run
     */
    virtual bool hasNextRun(
        const Context& context_) const noexcept = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_REPEATER_H_ */

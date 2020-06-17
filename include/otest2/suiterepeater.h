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

#ifndef OTest2_INCLUDE_OTEST2_SUITEREPEATER_H_
#define OTest2_INCLUDE_OTEST2_SUITEREPEATER_H_

#include <otest2/suiteptr.h>
#include <otest2/suiterepeaterptr.h>
#include <string>

namespace OTest2 {

class Context;

/**
 * @brief Generic interface of suite repeater
 *
 * The suite repeater is responsible for repeated run of a suite object.
 * For each run new instance of the suite is created. So actually the repeater
 * works as a suite factory. However, there might be a confusion - the suite
 * factory object creates instances of this suite repeater.
 */
class SuiteRepeater {
  public:
    /* -- avoid copying */
    SuiteRepeater(
        const SuiteRepeater&) = delete;
    SuiteRepeater& operator = (
        const SuiteRepeater&) = delete;

    /**
     * @brief Ctor
     */
    SuiteRepeater();

    /**
     * @brief Dtor
     */
    virtual ~SuiteRepeater();

    /**
     * @brief Check whether next run of the suite is planned
     *
     * @param context_ The otest2 context
     * @return True if next run is prepared
     */
    virtual bool isNextRun(
        const Context& context_) const = 0;

    /**
     * @brief Decorate suite name for next run
     * @param suitename_ Bare suite name
     * @return Decorated suite name
     * @warning The method may be called only if the isNextRun() is true!
     */
    virtual std::string transformName(
        const Context& context_,
        const std::string& suitename_) const = 0;

    /**
     * @brief Create suite
     *
     * @param context_ The OTest2 context
     * @param suitename_ Decorated suite name
     * @return The created suite
     * @warning The method may be called only if the isNextRun() is true!
     */
    virtual SuitePtr createSuite(
        const Context& context_,
        const std::string& suitename_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_SUITEREPEATER_H_ */

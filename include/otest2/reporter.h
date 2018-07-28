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

#ifndef OTest2__INCLUDE_OTEST2_REPORTER_H_
#define OTest2__INCLUDE_OTEST2_REPORTER_H_

#include <string>

namespace OTest2 {

class Context;

/**
 * @brief Generic test reporter
 */
class Reporter {
  public:
    /* -- avoid copying */
    Reporter(
        const Reporter&) = delete;
    Reporter& operator =(
        const Reporter&) = delete;

    /**
     * @brief Ctor
     */
    Reporter();

    /**
     * @brief Dtor
     */
    virtual ~Reporter();

    /**
     * @brief Enter entire test
     *
     * @param context_ The OTest2 context
     * @param name_ Name of the test
     */
    virtual void enterTest(
        const Context& context_,
        const std::string& name_) = 0;

    /**
     * @brief Enter a suite
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the suite
     */
    virtual void enterSuite(
        const Context& context_,
        const std::string& name_) = 0;

    /**
     * @brief Leave a suite
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the suite
     * @param result_ Result of the suite
     */
    virtual void leaveSuite(
        const Context& context_,
        const std::string& name_,
        bool result_) = 0;

    /**
     * @brief Leave entire test
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the leaving test
     * @param result_ Result of the test
     */
    virtual void leaveTest(
        const Context& context_,
        const std::string& name_,
        bool result_) = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_REPORTER_H_ */

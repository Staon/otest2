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

#ifndef OTest2_INCLUDE_OTEST2_CASEREPEATER_H_
#define OTest2_INCLUDE_OTEST2_CASEREPEATER_H_

#include <otest2/caseptr.h>
#include <otest2/caserepeaterptr.h>
#include <string>

namespace OTest2 {

class Context;

/**
 * @brief Generic interface of a testcase repeater
 *
 * The case repeater is responsible for repeated run of a testcase object.
 * For each run new instance of the case is created. So actually the repeater
 * works as a testcase factory. However, there might be a confusion - the case
 * factory object creates instances of this testcase repeater.
 */
class CaseRepeater {
  public:
    /* -- avoid copying */
    CaseRepeater(
        const CaseRepeater&) = delete;
    CaseRepeater& operator = (
        const CaseRepeater&) = delete;

    /**
     * @brief Ctor
     */
    CaseRepeater();

    /**
     * @brief Dtor
     */
    virtual ~CaseRepeater();

    /**
     * @brief Check whether next run of the suite is planned
     *
     * @param context_ The otest2 context
     * @return True if next run is prepared
     */
    virtual bool isNextRun(
        const Context& context_) const = 0;

    /**
     * @brief Decorate case name for next run
     * @param case_name_ Bare case name
     * @return Decorated case name
     * @warning The method may be called only if the isNextRun() is true!
     */
    virtual std::string transformName(
        const Context& context_,
        const std::string& case_name_) const = 0;

    /**
     * @brief Create next run of the test case
     *
     * @param context_ The OTest2 context
     * @param case_name_ Decorated case name
     * @return The created case
     * @warning The method may be called only if the isNextRun() is true!
     */
    virtual CasePtr createCase(
        const Context& context_,
        const std::string& case_name_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CASEREPEATER_H_ */

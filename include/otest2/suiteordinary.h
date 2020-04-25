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

#ifndef OTest2__INCLUDE_OTEST2_SUITEORDINARY_H_
#define OTest2__INCLUDE_OTEST2_SUITEORDINARY_H_

#include <string>

#include <otest2/casefactoryptr.h>
#include <otest2/suite.h>

namespace OTest2 {

class Context;

/**
 * @brief Ordinary suite
 *
 * This is a helper class of test suites following ordinary initialization,
 * run and destruction.
 */
class SuiteOrdinary : public Suite {
  public:
    /* -- avoid copying */
    SuiteOrdinary(
        const SuiteOrdinary&) = delete;
    SuiteOrdinary& operator =(
        const SuiteOrdinary&) = delete;

    /**
     * @brief Ctor
     *
     * @param context_ The otest2 context
     */
    explicit SuiteOrdinary(
        const Context& context_);

    /**
     * @brief Dtor
     */
    virtual ~SuiteOrdinary();

    /**
     * @brief Get a test case at specified index
     *
     * @param[in] context_ The OTest2 context
     * @param[in] index_ Index of the test case (zero based)
     * @param[out] name_ Filled name of the test case if it's found and
     *     the pointer is not null.
     * @return The test case or the nullptr if the index is out of the bounds.
     */
    virtual CaseFactoryPtr getCase(
        const Context& context_,
        int index_,
        std::string* name_) const = 0;

    /**
     * @brief Run a start-up function
     *
     * The data model expects a list of start-up functions. The functions
     * are invoked one by one. Start-up functions may represents a real
     * start-up function or an initialization function of a fixture object
     * or whatever else.
     *
     * The start-up functions are identified by a zero-based index with
     * no gaps.
     *
     * @param context_ The OTest2 context
     * @param index_ Zero based index of the start-up function
     * @return True if the start-up function is invoked. False if there is no
     *     start-up function at the index.
     */
    virtual bool startUpSuite(
        const Context& context_,
        int index_) = 0;

    /**
     * @brief Run a tear-down function
     *
     * The model expects a list of tear-down functions. The functions are
     * invoked one by one in decreasing order. A tear-down function may
     * represent e.g. a real tear-down function or a cleaning method of
     * a fixture object.
     *
     * The tear-down functions are identified by a zero-based index with
     * no gaps.
     *
     * @param context_ The otest2 context
     * @param index_ Index of the tear-down function
     */
    virtual void tearDownSuite(
        const Context& context_,
        int index_) = 0;

    /* -- suite */
    virtual void scheduleRun(
        const Context& context_,
        SuitePtr this_ptr_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_SUITEORDINARY_H_ */

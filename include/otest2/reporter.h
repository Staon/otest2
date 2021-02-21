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

#include <otest2/assertbufferptr.h>
#include <string>

namespace OTest2 {

class Context;
class Parameters;

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
     * @param params_ Parameters of the testing object
     */
    virtual void enterTest(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_) = 0;

    /**
     * @brief Enter a suite
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the suite
     * @param params_ Parameters of the testing object
     */
    virtual void enterSuite(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_) = 0;

    /**
     * @brief Enter a case
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the case
     * @param params_ Parameters of the testing object
     */
    virtual void enterCase(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_) = 0;

    /**
     * @brief Enter a state
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the state
     */
    virtual void enterState(
        const Context& context_,
        const std::string& name_) = 0;

    /**
     * @brief Enter an assertion
     *
     * The assertion is opened similarly to states, cases and suites until
     * the AsssertBuffer::commitAssertion() method of the returned
     * assertion buffer is invoked.
     *
     * @warning Just one assertion at time may be opened!
     *
     * @param context_ the OTest2 context
     * @param condition_ result of the assertion (false means failed assertion)
     * @param file_ name of the source file
     * @param lineno_ line number in the source file
     * @return An assertion buffer object which is used for formatting
     *     the assertion report.
     */
    virtual AssertBufferPtr enterAssert(
        const Context& context_,
        bool condition_,
        const std::string& file_,
        int lineno_) = 0;

    /**
     * @brief Enter an error report
     *
     * This method is used to report errors happened in the testing framework.
     * Errors mean failure of the test. However, they cannot be located in
     * the user code (no filename, no line number). A good example is
     * a not handled exception caught by the framework outside test cases.
     *
     * The method works as well as the enterAssert() method with failed condition
     * and with no location info. The opened error state is closed by the
     * AssertBuffer::commitAssertion() method of the returned assertion
     * buffer.
     *
     * @warning Just one assertion at time may be opened!
     *
     * @param context_ the context
     * @return An assertion buffer object which is used for formatting
     *     the assertion report.
     */
    virtual AssertBufferPtr enterError(
        const Context& context_) = 0;

    /**
     * @brief Leave a state
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the state
     * @param result_ Result of the state
     */
    virtual void leaveState(
        const Context& context_,
        const std::string& name_,
        bool result_) = 0;

    /**
     * @brief Leave a case
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the case
     * @param params_ Parameters of the testing object
     * @param result_ Result of the case
     */
    virtual void leaveCase(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_,
        bool result_) = 0;

    /**
     * @brief Leave a suite
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the suite
     * @param params_ Parameters of the testing object
     * @param result_ Result of the suite
     */
    virtual void leaveSuite(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_,
        bool result_) = 0;

    /**
     * @brief Leave entire test
     *
     * @param context_ the OTest2 context
     * @param name_ Name of the leaving test
     * @param params_ Parameters of the testing object
     * @param result_ Result of the test
     */
    virtual void leaveTest(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_,
        bool result_) = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_REPORTER_H_ */

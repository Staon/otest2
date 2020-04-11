/*
 * Copyright (C) 2019 Ondrej Starek
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

#ifndef OTest2__TEST_REPORTERMOCK_H_
#define OTest2__TEST_REPORTERMOCK_H_

#include <iosfwd>
#include <vector>

#include <otest2/assertbean.h>
#include <otest2/reporter.h>

namespace OTest2 {

namespace Test {

/**
 * @brief This is a mock reporter
 *
 * This class is used as a reported inside the self-tests. It stores reporter
 * events and allows to check them later.
 */
class ReporterMock : public Reporter {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    ReporterMock(
        const ReporterMock&) = delete;
    ReporterMock& operator =(
        const ReporterMock&) = delete;

    /**
     * @brief Ctor
     *
     * @param report_paths_ If it's true, current object path is reported too
     */
    explicit ReporterMock(
        bool report_paths_ = false);

    /**
     * @brief Dtor
     */
    virtual ~ReporterMock();

    /**
     * @brief Add a record for a delay between test steps
     *
     * @param delay_ms_ Delay in milliseconds
     */
    void reportDelay(
        int delay_ms_);

    /**
     * @brief Print the records into an output stream
     *
     * @param os_ The output stream
     * @return The output stream
     */
    std::ostream& dumpRecords(
        std::ostream& os_) const;

    /**
     * @brief Check whether the records match
     */
    AssertBean checkRecords(
        const std::vector<const char*>& data_) const;

    /* -- reporter interface */
    virtual void enterTest(
        const Context& context_,
        const std::string& name_);
    virtual void enterSuite(
        const Context& context_,
        const std::string& name_);
    virtual void enterCase(
        const Context& context_,
        const std::string& name_);
    virtual void enterState(
        const Context& context_,
        const std::string& name_);
    virtual void enterAssert(
        const Context& context_,
        bool condition_,
        const std::string& message_,
        const std::string& file_,
        int lineno_);
    virtual void enterError(
        const Context& context_,
        const std::string& message_);
    virtual void reportAssertionMessage(
        const Context& context_,
        const std::string& message_);
    virtual void leaveAssert(
        const Context& context_);
    virtual void leaveState(
        const Context& context_,
        const std::string& name_,
        bool result_);
    virtual void leaveCase(
        const Context& context_,
        const std::string& name_,
        bool result_);
    virtual void leaveSuite(
        const Context& context_,
        const std::string& name_,
        bool result_);
    virtual void leaveTest(
        const Context& context_,
        const std::string& name_,
        bool result_);
};

} /* namespace Test */

} /* namespace OTest2 */

#endif /* OTest2__TEST_REPORTERMOCK_H_ */

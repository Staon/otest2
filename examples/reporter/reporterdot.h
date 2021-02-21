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

#ifndef OTest2__EXAMPLES_REPORTER_REPORTERDOT_H_
#define OTest2__EXAMPLES_REPORTER_REPORTERDOT_H_

#include <memory>

#include <otest2/reporter.h>

namespace OTest2 {

namespace Examples {

/**
 * @brief A simple reporter printing a dot per each test case
 */
class ReporterDot : public Reporter {
  private:
    class Impl;
    std::unique_ptr<Impl> pimpl;

  public:
    /**
     * @brief Ctor
     */
    ReporterDot();

    /**
     * @brief Dtor
     */
    virtual ~ReporterDot();

    /* -- avoid copying */
    ReporterDot(
        const ReporterDot&) = delete;
    ReporterDot& operator = (
        const ReporterDot&) = delete;

    /* -- reporter interface */
    virtual void enterTest(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_) override;
    virtual void enterSuite(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_) override;
    virtual void enterCase(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_) override;
    virtual void enterState(
        const Context& context_,
        const std::string& name_) override;
    virtual AssertBufferPtr enterAssert(
        const Context& context_,
        bool condition_,
        const std::string& file_,
        int lineno_) override;
    virtual AssertBufferPtr enterError(
        const Context& context_) override;
    virtual void leaveState(
        const Context& context_,
        const std::string& name_,
        bool result_) override;
    virtual void leaveCase(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_,
        bool result_) override;
    virtual void leaveSuite(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_,
        bool result_) override;
    virtual void leaveTest(
        const Context& context_,
        const std::string& name_,
        const Parameters& params_,
        bool result_) override;
};

} /* -- namespace Examples */

} /* -- namespace OTest2 */

#endif /* -- OTest2__EXAMPLES_REPORTER_REPORTERDOT_H_ */

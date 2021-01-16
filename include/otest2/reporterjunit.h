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

#ifndef OTest2__INCLUDE_OTEST2_REPORTERJUNIT_H_
#define OTest2__INCLUDE_OTEST2_REPORTERJUNIT_H_

#include <string>

#include <otest2/reporter.h>

namespace OTest2 {

/**
 * @brief A reporter creating JUnit XML reports
 */
class ReporterJUnit : public Reporter {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     *
     * @param file_ filename of the generated XML report
     * @param hide_location_ If it's true the location information (file
     *     and line number is omitted in the generated XML file.
     */
    explicit ReporterJUnit(
        const std::string& file_,
        bool hide_location_);

    /**
     * @brief Dtor
     */
    virtual ~ReporterJUnit();

    /* -- avoid copying */
    ReporterJUnit(
        const ReporterJUnit&) = delete;
    ReporterJUnit& operator = (
        const ReporterJUnit&) = delete;

    /* -- reporter interface */
    virtual void enterTest(
        const Context& context_,
        const std::string& name_) override;
    virtual void enterSuite(
        const Context& context_,
        const std::string& name_) override;
    virtual void enterCase(
        const Context& context_,
        const std::string& name_) override;
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
        bool result_) override;
    virtual void leaveSuite(
        const Context& context_,
        const std::string& name_,
        bool result_) override;
    virtual void leaveTest(
        const Context& context_,
        const std::string& name_,
        bool result_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_REPORTERJUNIT_H_ */

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

#ifndef OTest2__INCLUDE_OTEST2_REPORTERDEBUG_H_
#define OTest2__INCLUDE_OTEST2_REPORTERDEBUG_H_

#include <iosfwd>

#include <otest2/reporter.h>

namespace OTest2 {

/**
 * @brief Debugging implementation of the Reporter interface
 */
class ReporterDebug : public Reporter {
  public:
    std::ostream* os;

    /* -- avoid copying */
    ReporterDebug(
        const ReporterDebug&) = delete;
    ReporterDebug& operator =(
        const ReporterDebug&) = delete;

    /**
     * @brief Ctor
     *
     * @param os_ An output stream. The ownership is not taken.
     */
    explicit ReporterDebug(
        std::ostream* os_);

    /**
     * @brief Dtor
     */
    virtual ~ReporterDebug();

    /* -- reporter interface */
    virtual void enterTest(
        const Context& context_,
        const std::string& name_);
    virtual void enterSuite(
        const Context& context_,
        const std::string& name_);
    virtual void leaveSuite(
        const Context& context_,
        const std::string& name_,
        bool result_);
    virtual void leaveTest(
        const Context& context_,
        const std::string& name_,
        bool result_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_REPORTERDEBUG_H_ */

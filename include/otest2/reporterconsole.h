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

#ifndef OTest2__INCLUDE_OTEST2_REPORTERCONSOLE_H_
#define OTest2__INCLUDE_OTEST2_REPORTERCONSOLE_H_

#include <iosfwd>

#include <otest2/reporter.h>

namespace OTest2 {

/**
 * @brief A reporter writing into a console or file
 *
 * This reporter writes the test results into a console.
 */
class ReporterConsole : public Reporter {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    ReporterConsole(
        const ReporterConsole&) = delete;
    ReporterConsole& operator =(
        const ReporterConsole&) = delete;

    /**
     * @brief Ctor
     *
     * @param out_ An output stream. The ownership is not taken.
     */
    explicit ReporterConsole(
        std::ostream* out_);

    /**
     * @brief Dtor
     */
    virtual ~ReporterConsole();

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
    virtual void reportAssert(
        const Context& context_,
        bool condition_,
        const std::string& message_,
        const std::string& file_,
        int lineno_);
    virtual void reportError(
        const Context& context_,
        const std::string& message_);
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

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_REPORTERCONSOLE_H_ */

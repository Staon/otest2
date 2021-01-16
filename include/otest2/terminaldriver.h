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

#ifndef OTest2__INCLUDE_OTEST2_TERMINALDRIVER_H_
#define OTest2__INCLUDE_OTEST2_TERMINALDRIVER_H_

#include <iosfwd>
#include <otest2/reporterattributes.h>

namespace OTest2 {

/**
 * @brief Terminal driver
 *
 * This is a driver object which allows manipulation with terminal attributes
 * like text color or boldness.
 */
class TerminalDriver {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    TerminalDriver(
        const TerminalDriver&) = delete;
    TerminalDriver& operator =(
        const TerminalDriver&) = delete;

    /**
     * @brief Ctor
     *
     * @param handle_ File handle of the output stream. The handle is used
     *     to detect whether the output is a terminal and for initialization
     *     of the terminfo library.
     */
    explicit TerminalDriver(
        int handle_);

    /**
     * @brief Dtor
     */
    ~TerminalDriver();

    /**
     * @brief Change the foreground color of the text
     *
     * @param buffer_ A stream buffer which the control sequence is written
     *     into.
     * @param color_ The new color of the text
     */
    void setForeground(
        std::streambuf& buffer_,
        Color color_);

    /**
     * @brief Change the text style
     *
     * @param buffer_ A stream buffer which the control sequence is written
     *     into.
     */
    void setTextStyle(
        std::streambuf& buffer_,
        Style style_);

    /**
     * @brief Clean all previously set terminal attributes
     *
     * @param buffer_ A stream buffer which the control sequence is written
     *     into.
     */
    void cleanAttributes(
        std::streambuf& buffer_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TERMINALDRIVER_H_ */

/*
 * Copyright (C) 2021 Ondrej Starek
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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTBUFFER_H_
#define OTest2_INCLUDE_OTEST2_ASSERTBUFFER_H_

#include <streambuf>
#include <otest2/assertbufferptr.h>
#include <otest2/reporterattributes.h>

namespace OTest2 {

class Context;

/**
 * @brief Streambuffer interface used in the assertion API
 */
class AssertBuffer : public std::streambuf {
  public:
    /* -- avoid copying */
    AssertBuffer(
        const AssertBuffer&) = delete;
    AssertBuffer& operator = (
        const AssertBuffer&) = delete;

    /**
     * @brief Ctor
     */
    AssertBuffer();

    /**
     * @brief Dtor
     */
    virtual ~AssertBuffer();

    /**
     * @brief Set foreground color
     *
     * @param color_ The color
     */
    virtual void setForeground(
        Color color_) = 0;

    /**
     * @brief Set style of the shown text
     *
     * @param style_ The style
     */
    virtual void setTextStyle(
        Style style_) = 0;

    /**
     * @brief Reset currently set text attributes
     */
    virtual void resetAttributes() = 0;

    /**
     * @brief Commit (flush) current assertion message
     *
     * @param context_ The OTest2 context
     */
    virtual void commitMessage(
        const Context& context_) = 0;

    /**
     * @brief Finish current assertion
     *
     * @param context_ The OTest2 context
     */
    virtual void commitAssertion(
        const Context& context_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTBUFFER_H_ */

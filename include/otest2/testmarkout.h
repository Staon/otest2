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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKOUT_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKOUT_H_

#include <cstdint>
#include <string>

namespace OTest2 {

/**
 * @brief A generic interface of a serializer of test marks
 */
class TestMarkOut {
  public:
    /**
     * @brief Ctor
     */
    TestMarkOut();

    /**
     * @brief Dtor
     */
    virtual ~TestMarkOut();

    /* -- avoid copying */
    TestMarkOut(
        const TestMarkOut&) = delete;
    TestMarkOut& operator = (
        const TestMarkOut&) = delete;

    /**
     * @brief Write type mark of currently stored testmark
     *
     * @param typemark_ The typemark. It's a unique string identifying
     *     the testmark type and consisting maximally of 255 characters.
     */
    virtual void writeTypeMark(
        const char* typemark_) = 0;

    /**
     * @brief Write a 64 bit integer
     *
     * @param value_ The integer
     */
    virtual void writeInt(
        std::int64_t value_) = 0;

    /**
     * @brief Write 64-bit float
     *
     * @param value_ The value
     */
    virtual void writeFloat(
        long double value_) = 0;

    /**
     * @brief Write text string
     *
     * @param string_ The string
     */
    virtual void writeString(
        const std::string& string_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TESTMARKOUT_H_ */

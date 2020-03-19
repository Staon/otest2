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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKIN_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKIN_H_

#include <cstdint>
#include <string>

#include <otest2/testmarkptr.h>

namespace OTest2 {

class TestMarkFactory;

/**
 * @brief A generic interface of a testmark deserializer
 */
class TestMarkIn {
  public:
    /**
     * @brief Ctor
     */
    TestMarkIn();

    /**
     * @brief Dtor
     */
    virtual ~TestMarkIn();

    /* -- avoid copying */
    TestMarkIn(
        const TestMarkIn&) = delete;
    TestMarkIn& operator = (
        const TestMarkIn&) = delete;

    /**
     * @brief Parse type mark following in the stream
     *
     * @return The type mark
     * @exception ExcTestMarkIn
     */
    virtual std::string readTypeMark() = 0;

    /**
     * @brief Parse integer following in the stream
     *
     * @return The integer
     * @exception ExcTestMarkIn
     */
    virtual std::int64_t readInt() = 0;

    /**
     * @brief Parse float following in the stream
     *
     * @return The float value
     * @exception ExcTestMarkIn
     */
    virtual long double readFloat() = 0;


    /**
     * @brief Parse string following in the stream
     *
     * @return The string
     * @exception ExcTestMarkIn
     */
    virtual std::string readString() = 0;

    /**
     * @brief Deserialize a testmark
     *
     * @param factory_ A factory used for creating of new testmarks
     * @param deserializer_ A deserializer object
     * @return Deserialized testmark
     * @exception ExcTestMarkIn
     */
    static TestMarkPtr deserialize(
        TestMarkFactory& factory_,
        TestMarkIn& deserializer_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TESTMARKIN_H_ */

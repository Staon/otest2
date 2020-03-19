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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKINBIN_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKINBIN_H_

#include <cstdint>
#include <initializer_list>

#include <otest2/testmarkin.h>

namespace OTest2 {

enum class TestMarkOutBinTag : std::uint8_t;

/**
 * @brief Testmark deserializer based on a binary stream
 */
class TestMarkInBin : public TestMarkIn {
  private:
    /**
     * @brief Read binary data from the stream
     *
     * @param[in] size_ Size of the buffer
     * @param[out] buffer_ an output buffer at least of the size @a size_
     */
    virtual void readBinaryData(
        std::uint64_t size_,
        std::uint8_t* buffer_) = 0;

    TestMarkOutBinTag readTag(
        std::initializer_list<TestMarkOutBinTag> tags_);

  public:
    /**
     * @brief Ctor
     */
    TestMarkInBin();

    /**
     * @brief Dtor
     */
    virtual ~TestMarkInBin();

    /* -- testmarkin */
    virtual std::string readTypeMark() override;
    virtual std::int64_t readInt() override;
    virtual long double readFloat() override;
    virtual std::string readString() override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TESTMARKINBIN_H_ */

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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKOUTBIN_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKOUTBIN_H_

#include <cstdint>

#include <otest2/testmarkout.h>

namespace OTest2 {

enum class TestMarkOutBinTag : std::uint8_t;

/**
 * @brief Generic implementation of the testmark serializer based on a binary
 *     stream
 */
class TestMarkOutBin : public TestMarkOut {
  private:
    /**
     * @brief Write a block of binary data
     *
     * @param size_ Size of the block (bytes)
     * @param data_ The block
     */
    virtual void writeBinaryData(
        std::uint64_t size_,
        const std::uint8_t* data_) = 0;

    void writeTag(
        TestMarkOutBinTag tag_);

  public:
    /**
     * @brief Ctor
     */
    TestMarkOutBin();

    /**
     * @brief Dtor
     */
    virtual ~TestMarkOutBin();

    /* -- avoid copying */
    TestMarkOutBin(
        const TestMarkOutBin&) = delete;
    TestMarkOutBin& operator = (
        const TestMarkOutBin&) = delete;

    /* -- testmark serializer */
    virtual void writeTypeMark(
        const char* typemark_) override;
    virtual void writeInt(
        std::int64_t value_) override;
    virtual void writeFloat(
        long double value_) override;
    virtual void writeString(
        const std::string& string_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TESTMARKOUTBIN_H_ */

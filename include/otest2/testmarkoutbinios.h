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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKOUTBINIOS_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKOUTBINIOS_H_

#include <iosfwd>

#include <otest2/testmarkoutbin.h>

namespace OTest2 {

/**
 * @brief A binary testmark serializer based on the standard C++ streams
 */
class TestMarkOutBinIOS : public TestMarkOutBin {
  private:
    std::ostream* os;

    /* -- the serializer interface */
    virtual void writeBinaryData(
        std::uint64_t size_,
        const std::uint8_t* data_) override;

  public:
    /**
     * @brief Ctor
     *
     * @param os_ The output stream. The ownership is not taken.
     */
    explicit TestMarkOutBinIOS(
        std::ostream* os_);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkOutBinIOS();

    /* -- avoid copying */
    TestMarkOutBinIOS(
        const TestMarkOutBinIOS&) = delete;
    TestMarkOutBinIOS& operator = (
        const TestMarkOutBinIOS&) = delete;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TESTMARKOUTBINIOS_H_ */

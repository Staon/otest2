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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKINBINIOS_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKINBINIOS_H_

#include <iosfwd>

#include <otest2/testmarkinbin.h>

namespace OTest2 {

/**
 * @brief Binary testmark deserializer based on a C++ input stream
 */
class TestMarkInBinIOS : public TestMarkInBin {
  private:
    std::istream* is;

    /* -- binary deserializer */
    virtual void readBinaryData(
        std::uint64_t size_,
        std::uint8_t* buffer_) override;

  public:
    /**
     * @brief Ctor
     *
     * @param is_ An input stream. The ownership is not taken.
     */
    explicit TestMarkInBinIOS(
        std::istream* is_);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkInBinIOS();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TESTMARKINBINIOS_H_ */

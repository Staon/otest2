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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKPRINTER_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKPRINTER_H_

#include <iosfwd>
#include <string>
#include <vector>

#include <otest2/testmark.h>

namespace OTest2 {

/**
 * @brief Printer of testmarks
 */
class TestMarkPrinter {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     *
     * @param[in] array_ An injected testmark array. The ownership is not
     *     taken.
     * @param[out] index_ A variable keeping current index in the array.
     *     Client code can observe current index. Client code must not
     *     change the value!
     */
    explicit TestMarkPrinter(
        const std::vector<TestMark::LinearizedRecord>* array_,
        int& index_);

    /**
     * @brief Dtor
     */
    ~TestMarkPrinter();

    /* -- avoid copying */
    TestMarkPrinter(
        const TestMarkPrinter&) = delete;
    TestMarkPrinter& operator =(
        const TestMarkPrinter&) = delete;

    /**
     * @brief Print current line into a stream
     *
     * @param os_ The output stream
     * @param prefix_ Prefix of the printed line
     * @return True if there are next items in the array
     */
    bool printLine(
        std::ostream& os_,
        const std::string& prefix_);

    /**
     * @brief Skip current line
     *
     * @param os_ The output stream. Although the line is skipped some
     *     closed nodes may be printed.
     * @return True if there are next items in the array
     */
    bool skipLine(
        std::ostream& os_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TESTMARKPRINTER_H_ */

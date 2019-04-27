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

#ifndef OTest2__LIB_TESTMARK_H_
#define OTest2__LIB_TESTMARK_H_

namespace OTest2 {

constexpr long double DEFAULT_FLOAT_PRECISION(1.0e-9);

/**
 * @brief Generic interface of a test mark node
 */
class TestMark {
  private:
    /**
     * @brief Check whether this mark is equal to the @a other_
     *
     * @param other_ The other mark. The mark can be safely retyped
     *     this type!
     * @param precision_ Precision of comparison of floats
     * @return True if the marks are the same
     */
    virtual bool doIsEqual(
        const TestMark& other_,
        long double precision_) const = 0;

  public:
    /**
     * @brief Ctor
     */
    TestMark();

    /**
     * @brief Dtor
     */
    virtual ~TestMark();

    /* -- avoid copying */
    TestMark(
        const TestMark&) = delete;
    TestMark& operator =(
        const TestMark&) = delete;

    /**
     * @brief Compare two marks for equality
     *
     * @param other_ The other mark
     * @param precision_ Optional precision of comparison of floating point
     *     numbers
     * @return True if the marks are equal
     */
    bool isEqual(
        const TestMark& other_,
        long double precision_ = DEFAULT_FLOAT_PRECISION) const;
};

} /* namespace OTest2 */

#endif /* OTest2__LIB_TESTMARK_H_ */

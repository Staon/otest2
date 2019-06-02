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

#include <string>
#include <vector>

namespace OTest2 {

class DiffLogBuilder;

constexpr long double DEFAULT_FLOAT_PRECISION(1.0e-9);

/**
 * @brief Generic interface of a test mark node
 */
class TestMark {
  public:
    struct DiffRecord {
        const TestMark* parent;
        const TestMark* me;
        std::string label;
    };

  protected:
    /**
     * @brief Push me into the diff array
     */
    void pushDiffMe(
        const TestMark* parent_,
        const std::string& label_,
        std::vector<DiffRecord> array_) const;

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

    /**
     * @brief Compare values of 2 test mark nodes
     *
     * This method compares just value of current node and the other
     * node. It doesn't traverse the test mark subtrees.
     *
     * @param other_ The other node. The node can be safely casted
     *     to this type!
     * @param precision_ Precision of comparison of floating point
     *     numbers
     * @return True if the values of the marks are equal
     */
    virtual bool doIsEqualValue(
        const TestMark& other_,
        long double precision_) const = 0;

  public:
    /**
     * @brief Create the array which is used as the input sequence of the diff
     *     algorithm
     *
     * @param[in] parent_ Parent of the node
     * @param[in] label_ Label of the node
     * @param[out] array_ The diff array
     */
    virtual void doDiffArray(
        const TestMark* parent_,
        const std::string label_,
        std::vector<DiffRecord> array_) const = 0;

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

    /**
     * @brief Compare values of 2 test mark nodes
     *
     * This method compares just value of current node and the other
     * node. It doesn't traverse the test mark subtrees.
     *
     * @param other_ The other node
     * @param precision_ Optional precision of comparison of floating point
     *     numbers
     * @return True if the values of the marks are equal
     */
    bool isEqualValue(
        const TestMark& other_,
        long double precision_ = DEFAULT_FLOAT_PRECISION) const;

    /**
     * @brief Compute difference of two test marks
     *
     * @param[in] other_ The second test mark
     * @param[out] diff_ Generated log
     */
    void computeDiff(
        const TestMark& other_,
        DiffLogBuilder& diff_) const;
};

} /* namespace OTest2 */

#endif /* OTest2__LIB_TESTMARK_H_ */

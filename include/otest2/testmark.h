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

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

#include <otest2/testmarkhashcode.h>

namespace OTest2 {

class DiffLogBuilder;

constexpr long double DEFAULT_FLOAT_PRECISION(1.0e-9);

/**
 * @brief Generic interface of a test mark node
 */
class TestMark {
  public:
    /**
     * @brief One item of linearized test mark
     */
    struct LinearizedRecord {
        const int level;
        const TestMark* me;
        std::string label;
    };

  private:
    TestMarkHashCode hash;

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

    /**
     * @brief Create diff array of children
     *
     * @param[in] level_ Nested level
     * @param[out] array_ The array
     */
    virtual void doDiffArray(
        int level_,
        std::vector<LinearizedRecord>& array_) const = 0;

  public:
    /**
     * @brief Create linearized test mark
     *
     * @param[in] level_ Nested level
     * @param[in] label_ Label of the mark
     * @param[out] array_ The array
     */
    virtual void doLinearizedMark(
        int level_,
        const std::string& label_,
        std::vector<LinearizedRecord>& array_) const = 0;

  private:
    /**
     * @copydoc printOpen()
     */
    virtual void doPrintOpen(
        std::ostream& os_,
        const std::string& prefix_) const = 0;

    /**
     * @copydoc printClose()
     */
    virtual void doPrintClose(
        std::ostream& os_,
        const std::string& prefix_) const = 0;

    static void computeDiff(
        int level_,
        const std::vector<LinearizedRecord>& left_,
        const std::vector<LinearizedRecord>& right_,
        std::vector<LinearizedRecord>& left_result_,
        std::vector<LinearizedRecord>& right_result_,
        DiffLogBuilder& diff_);

  public:
    /**
     * @brief Ctor
     */
    TestMark();

    /**
     * @brief Ctor
     *
     * @param hash_ Initial hash code
     */
    explicit TestMark(
        TestMarkHashCode hash_);

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
     * @brief Set testmark's hash
     * @warning Use it only if you know what your doing
     */
    void setHashCode(
        TestMarkHashCode code_);

    /**
     * @brief Get testmarks' hash code
     */
    TestMarkHashCode getHashCode() const noexcept;

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
     * @brief Compare values of 2 testmark nodes and their hash codes
     *
     * This method compares just value of current node and the value
     * of the @a other_ node. Furthermore, it compares the hash codes
     * of both nodes (it's a kind of weak comparison of the whole subtree).
     *
     * @param other_ The other node
     * @param precision_ Optional precision of comparison of floating point
     *     numbers
     * @return True if the values of the marks are equal
     */
    bool isEqualValueHash(
        const TestMark& other_,
        long double precision_ = DEFAULT_FLOAT_PRECISION) const;

    /**
     * @brief Create the linearized test mark
     *
     * @param[out] array_ The array
     */
    void linearizedMark(
        std::vector<LinearizedRecord>& array_) const;

    /**
     * @brief Print opening of the node into a stream
     *
     * @param os_ The stream
     * @param prefix_ Prefix just before the printed value
     */
    void printOpen(
        std::ostream& os_,
        const std::string& prefix_) const;

    /**
     * @brief Print closing of the node into a stream
     *
     * @param os_ The stream
     * @param prefix_ Prefix just before the printed value
     */
    void printClose(
        std::ostream& os_,
        const std::string& prefix_) const;

    /**
     * @brief Compute difference of two test marks
     *
     * @param[in] other_ The second test mark
     * @param[out] left_ Linearized this testmark
     * @param[out] right_ Linearized other testmark
     * @param[out] diff_ Generated log
     */
    void computeDiff(
        const TestMark& other_,
        std::vector<LinearizedRecord>& left_,
        std::vector<LinearizedRecord>& right_,
        DiffLogBuilder& diff_) const;

    /**
     * @brief Print the testmark
     *
     * @param os_ An output stream
     * @param prefix_ A prefix at the beginning of each line
     */
    void printMark(
        std::ostream& os_,
        const std::string& prefix_) const;
};

} /* namespace OTest2 */

#endif /* OTest2__LIB_TESTMARK_H_ */

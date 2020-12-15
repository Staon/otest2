/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2
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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTIONSTEXT_H_
#define OTest2_INCLUDE_OTEST2_ASSERTIONSTEXT_H_

#include <iosfwd>

#include <otest2/assertcontext.h>
#include <otest2/assertionannotation.h>

namespace OTest2 {

class LongTextAssertion : public AssertContext {
  private:
    bool testAssertImpl(
        std::istream& a_,
        std::istream& b_);

  public:
    /* -- avoid copying */
    LongTextAssertion(
        const LongTextAssertion&) = delete;
    LongTextAssertion& operator = (
        const LongTextAssertion&) = delete;

    /* -- inherit the constructor - the parent constructor is invoked
     *    from the test suite generated code. */
    using AssertContext::AssertContext;

    /* -- text assertions */
    bool testAssertLongTextSS(
        std::istream& a_,
        std::istream& b_);
    bool testAssertLongTextST(
        std::istream& a_,
        const std::string& b_);
    bool testAssertLongTextSF(
        std::istream& a_,
        const std::string& file_b_);
    bool testAssertLongTextTS(
        const std::string& a_,
        std::istream& b_);
    bool testAssertLongTextTT(
        const std::string& a_,
        const std::string& b_);
    bool testAssertLongTextTF(
        const std::string& a_,
        const std::string& file_b_);
    bool testAssertLongTextFS(
        const std::string& file_a_,
        std::istream& b_);
    bool testAssertLongTextFT(
        const std::string& file_a_,
        const std::string& b_);
    bool testAssertLongTextFF(
        const std::string& file_a_,
        const std::string& file_b_);
};

namespace Assertions {

/**
 * @brief Compare content of two streams
 *
 * This assertion reads content of two streams and compares them by the
 * standard diff algorithm (longest common subsequence). If the contents
 * differ the assertion fails.
 *
 * @warning As the assertion reads the entire content into memory, the streams
 *     must not be infinite. Generally, don't pass streams connected to terminal
 *     pipes or socket channels. Use ordinary files or memory streams.
 *
 * @param a_ Left stream
 * @param b_ Right stream
 * @return True if the contents are the same
 */
bool testAssertLongTextSS(
    std::istream& a_,
    std::istream& b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextSS);

/**
 * @brief Compare stream and text by the diff algorithm
 * @sa testAssertLongTextSS
 */
bool testAssertLongTextST(
    std::istream& a_,
    const std::string& b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextST);

/**
 * @brief Compare stream and file by the diff algorithm
 * @sa testAssertLongTextSS
 */
bool testAssertLongTextSF(
    std::istream& a_,
    const std::string& file_b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextSF);

/**
 * @brief Compare text and stream by the diff algorithm
 * @sa testAssertLongTextSS
 */
bool testAssertLongTextTS(
    const std::string& a_,
    std::istream& b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextTS);

/**
 * @brief Compare two texts by the diff algorithm
 * @sa testAssertLongTextSS
 */
bool testAssertLongTextTT(
    const std::string& a_,
    const std::string& b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextTT);

/**
 * @brief Compare text and file by the diff algorithm
 * @sa testAssertLongTextSS
 */
bool testAssertLongTextTF(
    const std::string& a_,
    const std::string& file_b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextTF);

/**
 * @brief Compare file and stream by the diff algorithm
 * @sa testAssertLongTextSS
 */
bool testAssertLongTextFS(
    const std::string& file_a_,
    std::istream& b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextFS);

/**
 * @brief Compare file and text by the diff algorithm
 * @sa testAssertLongTextSS
 */
bool testAssertLongTextFT(
    const std::string& file_a_,
    const std::string& b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextFT);

/**
 * @brief Compare two files by the diff algorithm
 * @sa testAssertLongTextSS
 */
bool testAssertLongTextFF(
    const std::string& file_a_,
    const std::string& file_b_)
TEST_ASSERTION_MARK(::OTest2::LongTextAssertion, testAssertLongTextFF);

} /* -- namespace Assertions */

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTIONSTEXT_H_ */

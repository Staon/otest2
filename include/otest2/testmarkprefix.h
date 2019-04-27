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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKPREFIX_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKPREFIX_H_

#include <string>

#include <otest2/testmark.h>

namespace OTest2 {

/**
 * @brief A generic test mark with string prefix
 */
class TestMarkPrefix : public TestMark {
  private:
    std::string prefix;

    virtual bool doIsEqual(
        const TestMark& other_,
        long double precision_) const;

    /**
     * @brief Check equality of the test mark while the prefix is equal
     *
     * @param other_ The other test mark
     * @param precision_ Precision of floating point equality
     * @return True if the marks are equal
     */
    virtual bool doIsEqualPrefixed(
        const TestMark& other_,
        long double precision_) const = 0;

  protected:
    /**
     * @brief Ctor
     *
     * @param prefix_ The prefix. It can be empty.
     */
    explicit TestMarkPrefix(
        const std::string& prefix_);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkPrefix();

  public:
    /* -- avoid copying */
    TestMarkPrefix(
        const TestMarkPrefix&) = delete;
    TestMarkPrefix& operator =(
        const TestMarkPrefix&) = delete;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TESTMARKPREFIX_H_ */

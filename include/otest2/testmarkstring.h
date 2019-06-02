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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKSTRING_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKSTRING_H_

#include <string>

#include <otest2/testmark.h>

namespace OTest2 {

/**
 * @brief String test mark
 */
class TestMarkString : public TestMark {
  private:
    std::string value;

    /* -- test mark interface */
    virtual bool doIsEqual(
        const TestMark& other_,
        long double precision_) const;
    virtual bool doIsEqualValue(
        const TestMark& other_,
        long double precision_) const;
    virtual void doDiffArray(
        const TestMark* parent_,
        const std::string label_,
        std::vector<DiffRecord> array_) const;

  public:
    /**
     * @brief Ctor
     *
     * @param value_ Value of the mark
     */
    explicit TestMarkString(
        const std::string& value_);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkString();

    /* -- avoid copying */
    TestMarkString(
        const TestMarkString&) = delete;
    TestMarkString& operator =(
        const TestMarkString&) = delete;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TESTMARKSTRING_H_ */

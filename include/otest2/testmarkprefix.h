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
    const char* serialize_type_mark;
    std::string prefix;

    virtual bool doIsEqual(
        const TestMark& other_,
        long double precision_) const;
    virtual bool doIsEqualValue(
        const TestMark& other_,
        long double precision_) const;
    virtual void doPrintOpen(
        std::ostream& os_,
        const std::string& prefix_) const;
    virtual void doPrintClose(
        std::ostream& os_,
        const std::string& prefix_) const;
    virtual void doSerializeMark(
        TestMarkOut& serializer_) const;

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

    /**
     * @brief Get parenthesis characters
     *
     * @return An array of two characters: first is an opening parenthesis
     *     the second one is a closing parenthesis.
     */
    virtual const char* getParenthesis() const = 0;

    /**
     * @brief Serialize container's items
     *
     * @param serializer_ A serializer
     */
    virtual void serializeItems(
        TestMarkOut& serializer_) const = 0;

  protected:
    /**
     * @brief Ctor
     *
     * @param serialize_type_mark_ A type mark used for serialization.
     *     The ownership is not taken!
     * @param prefix_ The prefix. It can be empty.
     */
    explicit TestMarkPrefix(
        const char* serialize_type_mark_,
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

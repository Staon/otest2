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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKBOOL_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKBOOL_H_

#include <otest2/testmark.h>

namespace OTest2 {

struct CtorMark;

/**
 * @brief Bool test mark
 */
class TestMarkBool : public TestMark {
  private:
    bool value;

    /* -- test mark interface */
    virtual TestMarkHashCode doGetHashCode() const noexcept;
    virtual bool doIsEqual(
        const TestMark& other_,
        long double precision_) const;
    virtual bool doIsEqualValue(
        const TestMark& other_,
        long double precision_) const;
    virtual void doDiffArray(
        int level_,
        std::vector<LinearizedRecord>& array_) const;
    virtual void doLinearizedMark(
        int level_,
        const std::string& label_,
        std::vector<LinearizedRecord>& array_) const;
    virtual void doPrintOpen(
        std::ostream& os_,
        const std::string& prefix_) const;
    virtual void doPrintClose(
        std::ostream& os_,
        const std::string& prefix_) const;
    virtual void doSerializeMark(
        TestMarkOut& serializer_) const;
    virtual void doDeserializeMark(
        TestMarkFactory& factory_,
        TestMarkIn& deserializer_);

  public:
    /**
     * @brief Ctor
     *
     * @param value_ The initial value
     */
    explicit TestMarkBool(
        bool value_);

    /**
     * @brief Deserialization ctor
     */
    explicit TestMarkBool(
        CtorMark*);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkBool();

    /* -- avoid copying */
    TestMarkBool(
        const TestMarkBool&) = delete;
    TestMarkBool& operator =(
        const TestMarkBool&) = delete;

    static const char* typeMark();
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TESTMARKBOOL_H_ */

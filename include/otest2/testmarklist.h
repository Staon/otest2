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
#ifndef OTest2__INCLUDE_OTEST2_TESTMARKLIST_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKLIST_H_

#include <vector>

#include <otest2/testmarkprefix.h>
#include <otest2/testmarkptr.h>

namespace OTest2 {

struct CtorMark;

/**
 * @brief Ordered list of nested test marks
 */
class TestMarkList : public TestMarkPrefix {
  private:
    typedef std::vector<TestMarkPtr> List;
    List list;

    /* -- prefixed test mark interface */
    virtual bool doIsEqualPrefixed(
        const TestMark& other_,
        long double precision_) const;
    virtual void doDiffArray(
        int level_,
        std::vector<LinearizedRecord>& array_) const;
    virtual void doLinearizedMark(
        int level_,
        const std::string& label_,
        std::vector<LinearizedRecord>& array_) const;
    virtual const char* getParenthesis() const;
    virtual void serializeItems(
        TestMarkOut& serializer_) const;
    virtual void deserializeItems(
        TestMarkFactory& factory_,
        TestMarkIn& deserializer_);

  public:
    /**
     * @brief Ctor
     */
    TestMarkList();

    /**
     * @brief Ctor
     *
     * @param prefix_ Test mark prefix
     */
    explicit TestMarkList(
        const std::string& prefix_);

    /**
     * @brief Deserialization ctor
     */
    explicit TestMarkList(
        CtorMark*);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkList();

    /* -- avoid copying */
    TestMarkList(
        const TestMarkList&) = delete;
    TestMarkList& operator =(
        const TestMarkList&) = delete;

    /**
     * @brief Get serialization typemark
     */
    static const char* typeMark();

    /**
     * @brief Append a test mark into the list
     *
     * @param mark_ The test mark
     */
    void append(
        TestMarkPtr mark_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TESTMARKLIST_H_ */

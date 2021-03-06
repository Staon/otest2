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
#ifndef OTest2__INCLUDE_OTEST2_TESTMARKMAP_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKMAP_H_

#include <map>
#include <string>

#include <otest2/testmarkprefix.h>
#include <otest2/testmarkptr.h>

namespace OTest2 {

struct CtorMark;

/**
 * @brief Unordered indexed multimap of test marks
 */
class TestMarkMap : public TestMarkPrefix {
  private:
    typedef std::multimap<std::string, TestMarkPtr> Map;
    Map map;

    /* -- test mark interface */
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
    TestMarkMap();

    /**
     * @brief Ctor
     *
     * @param prefix_ Prefix of the test mark
     */
    explicit TestMarkMap(
        const std::string& prefix_);

    /**
     * @brief Deserialization ctor
     */
    explicit TestMarkMap(
        const CtorMark*);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkMap();

    /* -- avoid copying */
    TestMarkMap(
        const TestMarkMap&) = delete;
    TestMarkMap& operator = (
        const TestMarkMap&) = delete;

    /**
     * @brief Get serialization typemark
     */
    static const char* typeMark();

    /**
     * @brief Append a mark
     *
     * @param key_ Key of the mark
     * @param mark_ The mark
     */
    void append(
        const std::string& key_,
        TestMarkPtr mark_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TESTMARKMAP_H_ */

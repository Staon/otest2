/*
 * Copyright (C) 2021 Ondrej Starek
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

#ifndef OTest2_INCLUDE_OTEST2_TESTMARKFORMATTER_H_
#define OTest2_INCLUDE_OTEST2_TESTMARKFORMATTER_H_

#include <string>

namespace OTest2 {

class TestMark;

/**
 * @brief Generic interface of the testmark formatter
 */
class TestMarkFormatter {
  public:
    /* -- avoid copying */
    TestMarkFormatter(
        const TestMarkFormatter&) = delete;
    TestMarkFormatter& operator = (
        const TestMarkFormatter&) = delete;

    /**
     * @brief Ctor
     */
    TestMarkFormatter();

    /**
     * @brief Dtor
     */
    virtual ~TestMarkFormatter();

    /**
     * @brief Print opening of a testmark
     *
     * @param label_ Label of the testmark
     * @param mark_ The testmark
     * @param indent_ Indentation level
     */
    virtual void openMark(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) = 0;

    /**
     * @brief Print closing of a testmark
     *
     * @param label_ Label of the testmark
     * @param mark_ The mark
     * @param indent_ Indentation level
     */
    virtual void closeMark(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) = 0;

    /**
     * @brief Print opening of a deleted testmark
     *
     * @param label_ Label of the testmark
     * @param mark The testmark
     * @param indent_ Indentation level
     */
    virtual void openDeleted(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) = 0;

    /**
     * @brief Print closing of a deleted testmark
     *
     * @param label_ Label of the testmark
     * @param mark_ The testmark
     * @param indent_ Indentation level
     */
    virtual void closeDeleted(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) = 0;

    /**
     * @brief Print opening of an added testmark
     *
     * @param label_ Label of the testmark
     * @param mark_ The testmark
     * @param indent_ Indentation level
     */
    virtual void openAdded(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) = 0;

    /**
     * @brief Print closing of an added testmark
     *
     * @param label_ Label of the testmark
     * @param mark_ The testmark
     * @param indent_ Indentation level
     */
    virtual void closeAdded(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) = 0;

    /**
     * @brief Print separator (used for unchanged blocks)
     */
    virtual void printSeparator() = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_TESTMARKFORMATTER_H_ */

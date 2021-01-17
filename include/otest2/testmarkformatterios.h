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

#ifndef OTest2_INCLUDE_OTEST2_TESTMARKFORMATTERIOS_H_
#define OTest2_INCLUDE_OTEST2_TESTMARKFORMATTERIOS_H_

#include <iosfwd>
#include <otest2/reporterattributes.h>
#include <otest2/testmarkformatter.h>
#include <string>

namespace OTest2 {

/**
 * @brief Implementation of the testmark formatter based on an IO stream
 */
class TestMarkFormatterIOS : public TestMarkFormatter {
  private:
    std::ostream* os;
    std::string prefix;

    void printLine(
        const std::string& prefix_,
        const std::string& label_,
        const TestMark* mark_,
        int indent_,
        bool open_);

    virtual void setTextForeground(
        Color color_);
    virtual void setTextStyle(
        Style style_);
    virtual void resetTextAttributes();

  public:
    /* -- avoid copying */
    TestMarkFormatterIOS(
        const TestMarkFormatterIOS&) = delete;
    TestMarkFormatterIOS& operator = (
        const TestMarkFormatterIOS&) = delete;

    /**
     * @brief Ctor
     *
     * @param os_ An output stream. The ownership is not taken.
     * @param prefix_ A prefix prepended to each line
     */
    explicit TestMarkFormatterIOS(
        std::ostream* os_,
        const std::string& prefix_);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkFormatterIOS();

    /* -- testmark formatter */
    virtual void openMark(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) override;
    virtual void closeMark(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) override;
    virtual void openDeleted(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) override;
    virtual void closeDeleted(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) override;
    virtual void openAdded(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) override;
    virtual void closeAdded(
        const std::string& label_,
        const TestMark* mark_,
        int indent_) override;
    virtual void printSeparator() override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_TESTMARKFORMATTERIOS_H_ */

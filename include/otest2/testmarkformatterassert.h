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

#ifndef OTest2_INCLUDE_OTEST2_TESTMARKFORMATTERASSERT_H_
#define OTest2_INCLUDE_OTEST2_TESTMARKFORMATTERASSERT_H_

#include <otest2/testmarkformatterios.h>
#include <string>

namespace OTest2 {

class AssertStream;

/**
 * @brief Testmark formatter working with the assertion stream
 */
class TestMarkFormatterAssert : public TestMarkFormatterIOS {
  private:
    AssertStream* stream;

    virtual void setTextForeground(
        Color color_) override;
    virtual void setTextStyle(
        Style style_) override;
    virtual void resetTextAttributes() override;

  public:
    /* -- avoid copying */
    TestMarkFormatterAssert(
        const TestMarkFormatterAssert&) = delete;
    TestMarkFormatterAssert& operator = (
        const TestMarkFormatterAssert&) = delete;

    /**
     * @brief Ctor
     *
     * @param stream_ The assertion stream. The ownership is not taken.
     * @param prefix_ A prefix prepended to each line
     */
    explicit TestMarkFormatterAssert(
        AssertStream* stream_,
        const std::string& prefix_);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkFormatterAssert();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_TESTMARKFORMATTERASSERT_H_ */

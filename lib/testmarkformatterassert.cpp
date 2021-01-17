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

#include <testmarkformatterassert.h>

#include <assert.h>

#include <assertstream.h>

namespace OTest2 {

TestMarkFormatterAssert::TestMarkFormatterAssert(
    AssertStream* stream_,
    const std::string& prefix_) :
  TestMarkFormatterIOS(stream_, prefix_),
  stream(stream_) {
  assert(stream != nullptr);

}

TestMarkFormatterAssert::~TestMarkFormatterAssert() = default;

void TestMarkFormatterAssert::setTextForeground(
    Color color_) {
  *stream << foreground(color_);
}

void TestMarkFormatterAssert::setTextStyle(
    Style style_) {
  *stream << textStyle(style_);
}

void TestMarkFormatterAssert::resetTextAttributes() {
  *stream << resetAttrs();
}

} /* -- namespace OTest2 */

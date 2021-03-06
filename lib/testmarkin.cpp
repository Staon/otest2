/*
 * Copyright (C) 2020 Ondrej Starek
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

#include <testmarkin.h>

#include <string>

#include <testmark.h>
#include <testmarkfactory.h>

namespace OTest2 {

TestMarkIn::TestMarkIn() {

}

TestMarkIn::~TestMarkIn() {

}

TestMarkPtr TestMarkIn::deserialize(
    TestMarkFactory& factory_,
    TestMarkIn& deserializer_) {
  std::string typemark_(deserializer_.readTypeMark());
  TestMarkPtr testmark_(factory_.createMark(typemark_));
  testmark_->deserializeMark(factory_, deserializer_);
  return testmark_;
}

} /* -- namespace OTest2 */

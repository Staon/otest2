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
#include <testmarkmap.h>

#include <assert.h>

namespace OTest2 {

TestMarkMap::TestMarkMap() :
  TestMarkPrefix(""),
  map() {

}

TestMarkMap::TestMarkMap(
    const std::string& prefix_) :
  TestMarkPrefix(prefix_),
  map() {

}

TestMarkMap::~TestMarkMap() {

}

bool TestMarkMap::doIsEqualPrefixed(
    const TestMark& other_,
    long double precision_) const {
  const TestMarkMap* o_(static_cast<const TestMarkMap*>(&other_));

  if(map.size() != o_->map.size())
    return false;

  auto liter_(map.begin());
  auto riter_(o_->map.begin());
  for(; liter_ != map.end(); ++liter_, ++riter_) {
    if((*liter_).first != (*riter_).first)
      return false;
    if((*liter_).second->isEqual(*(*riter_).second, precision_))
      return false;
  }

  return true;
}

void TestMarkMap::append(
    const std::string& key_,
    TestMarkPtr mark_) {
  assert(!key_.empty() && mark_ != nullptr);

  map.insert(Map::value_type(key_, mark_));
}

} /* namespace OTest2 */

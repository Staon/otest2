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
#include <testmarklist.h>

#include <assert.h>

namespace OTest2 {

TestMarkList::TestMarkList() :
  TestMarkPrefix(""),
  list() {

}

TestMarkList::TestMarkList(
    const std::string& prefix_) :
  TestMarkPrefix(prefix_) {

}

TestMarkList::~TestMarkList() {

}

bool TestMarkList::doIsEqualPrefixed(
    const TestMark& other_,
    long double precision_) const {
  const TestMarkList* o_(static_cast<const TestMarkList*>(&other_));
  if(list.size() != o_->list.size())
    return false;
  for(int i_(0); i_ < list.size(); ++i_)
    if(!list[i_]->isEqual(*o_->list[i_], precision_))
      return false;
  return true;
}

void TestMarkList::doDiffArray(
    int level_,
    std::vector<LinearizedRecord>& array_) const
{
  for(const auto& item_ : list) {
    array_.push_back({level_, item_.get(), ""});
  }
}

void TestMarkList::doLinearizedMark(
    int level_,
    const std::string& label_,
    std::vector<LinearizedRecord>& array_) const {
  array_.push_back({level_, this, label_});
  for(const auto& item_ : list) {
    item_->doLinearizedMark(level_ + 1, "", array_);
  }
}

const char* TestMarkList::getParenthesis() const {
  return "[]";
}

void TestMarkList::append(
    TestMarkPtr mark_) {
  assert(mark_ != nullptr);

  list.push_back(mark_);
}

} /* namespace OTest2 */

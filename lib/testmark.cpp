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
#include <testmark.h>

#include <typeinfo>

#include "hirschberg.h"

namespace OTest2 {

namespace {

class TestMarkScore {
  public:
    int scoreSub(
        const TestMark::DiffRecord& left_,
        const TestMark::DiffRecord& right_,
        DiffAction action_,
        bool& same_) const {
      same_ = left_.me->isEqualValue(*right_.me);
      return same_ ? 1 : -1;
    }

    int scoreDel(
        const TestMark::DiffRecord& right_,
        DiffAction action_) const {
      return -1;
    }

    int scoreIns(
        const TestMark::DiffRecord& left_,
        DiffAction action_) const {
      return -1;
    }

    bool betterScore(
        int score1_,
        int score2_) const {
      return score1_ > score2_;
    }
};

} /* -- namespace */

TestMark::TestMark() {

}

TestMark::~TestMark() {

}

void TestMark::pushDiffMe(
    const TestMark* parent_,
    const std::string& label_,
    std::vector<DiffRecord> array_) const {
  array_.push_back({parent_, this, label_});
}

bool TestMark::isEqual(
    const TestMark& other_,
    long double precision_) const {
  if(typeid(this) == typeid(&other_))
    return doIsEqual(other_, precision_);
  else
    return false;
}

bool TestMark::isEqualValue(
    const TestMark& other_,
    long double precision_) const {
  if(typeid(this) == typeid(&other_))
    return doIsEqualValue(other_, precision_);
  else
    return false;
}

void TestMark::computeDiff(
    const TestMark& other_,
    DiffLogBuilder& diff_) const {
  /* -- prepare input array */
  std::vector<DiffRecord> left_;
  doDiffArray(nullptr, "", left_);
  std::vector<DiffRecord> right_;
  other_.doDiffArray(nullptr, "", right_);

  /* -- compute the diff */
  hirschbergDiff(left_, right_, diff_, TestMarkScore());
}

} /* namespace OTest2 */

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

#include <assert.h>
#include <string>
#include <typeinfo>

#include <otest2/difflogarray.h>
#include <otest2/hirschberg.h>
#include <otest2/testmarkprinter.h>

namespace OTest2 {

namespace {

class TestMarkScore {
  public:
    std::tuple<bool, int> scoreSub(
        int step_,
        const TestMark::LinearizedRecord left_[],
        int left_index_,
        const TestMark::LinearizedRecord right_[],
        int right_index_) const {
      /* -- check equality of both nodes */
      const TestMark::LinearizedRecord& lrec_(left_[left_index_]);
      const TestMark::LinearizedRecord& rrec_(right_[right_index_]);
      auto same_(lrec_.me->isEqualValueHash(*rrec_.me)
          && lrec_.label == rrec_.label);
      if(same_)
        return std::make_tuple(true, 1);
      else
        return std::make_tuple(false, -1);
    }

    int scoreDel(
        int step_,
        const TestMark::LinearizedRecord right_[],
        int right_index_) const {
      return -1;
    }

    int scoreIns(
        int step_,
        const TestMark::LinearizedRecord left_[],
        int left_index_) const {
      return -1;
    }

    bool betterScore(
        int score1_,
        int score2_) const {
      return score1_ > score2_;
    }
};

} /* -- namespace */

TestMark::TestMark() :
  hash(0) {

}

TestMark::TestMark(
    TestMarkHashCode hash_) :
  hash(hash_) {

}

TestMark::~TestMark() {

}

void TestMark::setHashCode(
    TestMarkHashCode code_) {
  hash = code_;
}

TestMarkHashCode TestMark::getHashCode() const noexcept {
  return hash;
}

bool TestMark::isEqual(
    const TestMark& other_,
    long double precision_) const {
  if(typeid(*this) == typeid(other_))
    return doIsEqual(other_, precision_);
  else
    return false;
}

bool TestMark::isEqualValue(
    const TestMark& other_,
    long double precision_) const {
  if(typeid(*this) == typeid(other_))
    return doIsEqualValue(other_, precision_);
  else
    return false;
}

bool TestMark::isEqualValueHash(
    const TestMark& other_,
    long double precision_) const {
  if(typeid(*this) != typeid(other_))
    return false;
  if (hash != other_.hash)
    return false;
  return doIsEqualValue(other_, precision_);
}

void TestMark::linearizedMark(
    std::vector<LinearizedRecord>& array_) const {
  doLinearizedMark(0, "", array_);
}

void TestMark::printOpen(
    std::ostream& os_,
    const std::string& prefix_) const {
  doPrintOpen(os_, prefix_);
}

void TestMark::printClose(
    std::ostream& os_,
    const std::string& prefix_) const {
  doPrintClose(os_, prefix_);
}

void TestMark::serializeMark(
    TestMarkOut& serializer_) const {
  doSerializeMark(serializer_);
}

void TestMark::computeDiff(
    int level_,
    const std::vector<LinearizedRecord>& left_,
    const std::vector<LinearizedRecord>& right_,
    std::vector<LinearizedRecord>& left_result_,
    std::vector<LinearizedRecord>& right_result_,
    DiffLogBuilder& diff_) {
  /* -- compute difference of current level */
  DiffLogArray diff_array_;
  DiffLogBuilderArray diff_level_(&diff_array_);
  hirschbergDiff(left_, right_, diff_level_, TestMarkScore());

  int left_index_(0);
  int right_index_(0);
  std::vector<LinearizedRecord> left_nested_;
  std::vector<LinearizedRecord> right_nested_;
  for(const auto& diff_rec_ : diff_array_) {
    /* -- copy unchanged records */
    int left_term_;
    switch(diff_rec_.action) {
      case DiffAction::CHANGE:
      case DiffAction::INSERT:
        left_term_ = diff_rec_.left_index;
        break;
      case DiffAction::DELETE:
        left_term_ = left_index_ + diff_rec_.right_index - right_index_;
        break;
      default:
        assert(false);
        break;
    }
    for(; left_index_ < left_term_; ++left_index_, ++right_index_) {
      left_result_.push_back(left_[left_index_]);
      right_result_.push_back(right_[right_index_]);
      diff_.addMatch(left_result_.size() - 1, right_result_.size() - 1);

      /* -- nest into next level */
      left_nested_.clear();
      right_nested_.clear();
      left_[left_index_].me->doDiffArray(level_ + 1, left_nested_);
      right_[right_index_].me->doDiffArray(level_ + 1, right_nested_);
      computeDiff(
          level_ + 1,
          left_nested_,
          right_nested_,
          left_result_,
          right_result_,
          diff_);
    }

    /* -- process the change */
    switch(diff_rec_.action) {
      case DiffAction::CHANGE:
        left_result_.push_back(left_[left_index_]);
        right_result_.push_back(right_[right_index_]);

        /* -- Distinguish a container with changed content but not values.
         *    If the containers are the same (same prefixes) I don't want
         *    to show just changes inside the container, not the opening
         *    diff line. */
        if(left_[left_index_].me->isEqualValue(*right_[right_index_].me))
          diff_.addMatch(left_result_.size() - 1, right_result_.size() - 1);
        else
          diff_.addChange(left_result_.size() - 1, right_result_.size() - 1);

        /* -- nest into next level */
        left_nested_.clear();
        right_nested_.clear();
        left_[left_index_].me->doDiffArray(level_ + 1, left_nested_);
        right_[right_index_].me->doDiffArray(level_ + 1, right_nested_);
        computeDiff(
            level_ + 1,
            left_nested_,
            right_nested_,
            left_result_,
            right_result_,
            diff_);

        ++left_index_;
        ++right_index_;

        break;

      case DiffAction::INSERT:
        left_result_.push_back(left_[left_index_]);
        diff_.addInsert(left_result_.size() - 1);

        /* -- nest into next level */
        left_nested_.clear();
        right_nested_.clear();
        left_[left_index_].me->doDiffArray(level_ + 1, left_nested_);
        computeDiff(
            level_ + 1,
            left_nested_,
            right_nested_,
            left_result_,
            right_result_,
            diff_);

        ++left_index_;

        break;
      case DiffAction::DELETE:
        right_result_.push_back(right_[right_index_]);
        diff_.addDelete(right_result_.size() - 1);

        /* -- nest into next level */
        left_nested_.clear();
        right_nested_.clear();
        right_[right_index_].me->doDiffArray(level_ + 1, right_nested_);
        computeDiff(
            level_ + 1,
            left_nested_,
            right_nested_,
            left_result_,
            right_result_,
            diff_);

        ++right_index_;

        break;
      default:
        assert(false);
        break;
    }
  }

  /* -- copy remaining unchanged records */
  for(; left_index_ < left_.size(); ++left_index_, ++right_index_) {
    assert(right_index_ < right_.size());

    left_result_.push_back(left_[left_index_]);
    right_result_.push_back(right_[right_index_]);
    diff_.addMatch(left_result_.size() - 1, right_result_.size() - 1);

    /* -- nest into next level */
    left_nested_.clear();
    right_nested_.clear();
    left_[left_index_].me->doDiffArray(level_ + 1, left_nested_);
    right_[right_index_].me->doDiffArray(level_ + 1, right_nested_);
    computeDiff(
        level_ + 1,
        left_nested_,
        right_nested_,
        left_result_,
        right_result_,
        diff_);
  }
  assert(right_index_ == right_.size());
}

void TestMark::computeDiff(
    const TestMark& other_,
    std::vector<LinearizedRecord>& left_,
    std::vector<LinearizedRecord>& right_,
    DiffLogBuilder& diff_) const {
  /* -- prepare input arrays */
  std::vector<LinearizedRecord> left_array_;
  left_array_.push_back({0, this, ""});
  std::vector<LinearizedRecord> right_array_;
  right_array_.push_back({0, &other_, ""});

  /* -- compute the diff */
  std::vector<LinearizedRecord> left_result_;
  std::vector<LinearizedRecord> right_result_;
  computeDiff(0, left_array_, right_array_, left_result_, right_result_, diff_);

  /* -- return the results */
  left_.swap(left_result_);
  right_.swap(right_result_);
}

void TestMark::printMark(
    std::ostream& os_,
    const std::string& prefix_) const {
  std::vector<LinearizedRecord> array_;
  linearizedMark(array_);
  int index_;
  TestMarkPrinter printer_(&array_, index_);
  while(printer_.printLine(os_, prefix_));
}

} /* namespace OTest2 */

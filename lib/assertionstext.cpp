/*
 * Copyright (C) 2020 Ondrej Starek
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
#include <otest2/assertionstext.h>

#include <assert.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <otest2/difflogblock.h>
#include <otest2/hirschberg.h>

namespace OTest2 {

namespace {

void slurpFile(
    std::vector<std::string>& data_,
    std::istream& is_) {
  for(std::string line_; std::getline(is_, line_);) {
    data_.push_back(std::move(line_));
  }
}

std::string formatLine(
    int left_line_,
    int right_line_,
    char change_,
    const std::string& line_) {
  std::ostringstream oss_;
  oss_ << std::setfill('0');

  /* -- left line number */
  if(left_line_ >= 0)
    oss_ << std::setw(4) << (left_line_ + 1);
  else
    oss_ << "    ";

  oss_ << ' ';

  /* -- right line number */
  if(right_line_ >= 0)
    oss_ << std::setw(4) << (right_line_ + 1);
  else
    oss_ << "    ";

  oss_ << " " << change_ << " : " << line_;

  return oss_.str();
}

} /* -- namespace */

bool LongTextAssertion::testAssertImpl(
    std::istream& a_,
    std::istream& b_) {
  /* -- read contents of the files */
  std::vector<std::string> a_data_;
  slurpFile(a_data_, a_);
  std::vector<std::string> b_data_;
  slurpFile(b_data_, b_);

  /* -- compute the difference */
  DiffLogBlocks diff_log_;
  DiffLogBuilderBlock log_builder_(&diff_log_);
  hirschbergDiff(a_data_, b_data_, log_builder_);

  bool result_(diff_log_.empty());
  if(result_) {
    /* -- There is no difference, the files match */
    enterAssertion(result_, "the texts are equal", false);
  }
  else {
    /* -- the files are different */
    enterAssertion(result_, "the texts are different", false);

    /* -- print the difference */
    constexpr int CONTEXT(3);
    bool trailing_context_(false);
    int left_line_(0);
    int right_line_(0);
    std::ostringstream oss_;
    for(const auto& difference_ : diff_log_) {
      /* -- print trailing context of previous change */
      if(trailing_context_) {
        const int context_end_(left_line_ + CONTEXT);
        for(;
            left_line_ < difference_.left_begin && left_line_ < context_end_;
            ++left_line_, ++right_line_) {
          assert(right_line_ < difference_.right_begin);

          assertionMessage(
              result_,
              formatLine(left_line_, right_line_, ' ', a_data_[left_line_]));
        }
      }
      else
        trailing_context_ = true;

      /* -- skip unchanged lines */
      if(left_line_ < difference_.left_begin - CONTEXT) {
        assert(right_line_ < difference_.right_begin);

        assertionMessage(result_, "........................................");
        left_line_ = difference_.left_begin - CONTEXT;
        right_line_ = difference_.right_begin - CONTEXT;
      }

      /* -- print entering context of the change */
      for(;
          left_line_ < difference_.left_begin;
          ++left_line_, ++right_line_) {
        assert(right_line_ < difference_.right_begin);

        assertionMessage(
            result_,
            formatLine(left_line_, right_line_, ' ', a_data_[left_line_]));
      }

      /* -- print the difference */
      assert(left_line_ == difference_.left_begin);
      assert(right_line_ == difference_.right_begin);
      for(; left_line_ < difference_.left_end; ++left_line_) {
        assertionMessage(
            result_,
            formatLine(left_line_, -1, '<', a_data_[left_line_]));
      }
      for(; right_line_ < difference_.right_end; ++right_line_) {
        assertionMessage(
            result_,
            formatLine(-1, right_line_, '>', b_data_[right_line_]));
      }
    }

    /* -- print ending context */
    const int context_end_(left_line_ + CONTEXT);
    for(;
        left_line_ < a_data_.size() && left_line_ < context_end_;
        ++left_line_, ++right_line_) {
      assert(right_line_ < b_data_.size());

      assertionMessage(
          result_,
          formatLine(left_line_, right_line_, ' ', a_data_[left_line_]));
    }

    /* -- skipt the end of the file */
    if(left_line_ < a_data_.size()) {
      assert(right_line_ < b_data_.size());

      assertionMessage(result_, "........................................");
    }
  }
  return leaveAssertion(result_);
}

bool LongTextAssertion::testAssertLongTextSS(
    std::istream& a_,
    std::istream& b_) {
  return testAssertImpl(a_, b_);
}

bool LongTextAssertion::testAssertLongTextST(
    std::istream& a_,
    const std::string& b_) {
  std::istringstream str_b_(b_);
  return testAssertImpl(a_, str_b_);
}

bool LongTextAssertion::testAssertLongTextSF(
    std::istream& a_,
    const std::string& file_b_) {
  std::ifstream str_b_(file_b_);
  return testAssertImpl(a_, str_b_);
}

bool LongTextAssertion::testAssertLongTextTS(
    const std::string& a_,
    std::istream& b_) {
  std::istringstream str_a_(a_);
  return testAssertImpl(str_a_, b_);
}

bool LongTextAssertion::testAssertLongTextTT(
    const std::string& a_,
    const std::string& b_) {
  std::istringstream str_a_(a_);
  std::istringstream str_b_(b_);
  return testAssertImpl(str_a_, str_b_);
}

bool LongTextAssertion::testAssertLongTextTF(
    const std::string& a_,
    const std::string& file_b_) {
  std::istringstream str_a_(a_);
  std::ifstream str_b_(file_b_);
  return testAssertImpl(str_a_, str_b_);
}

bool LongTextAssertion::testAssertLongTextFS(
    const std::string& file_a_,
    std::istream& b_) {
  std::ifstream str_a_(file_a_);
  return testAssertImpl(str_a_, b_);
}

bool LongTextAssertion::testAssertLongTextFT(
    const std::string& file_a_,
    const std::string& b_) {
  std::ifstream str_a_(file_a_);
  std::istringstream str_b_(b_);
  return testAssertImpl(str_a_, str_b_);
}

bool LongTextAssertion::testAssertLongTextFF(
    const std::string& file_a_,
    const std::string& file_b_) {
  std::ifstream str_a_(file_a_);
  std::ifstream str_b_(file_b_);
  return testAssertImpl(str_a_, str_b_);
}

} /* -- namespace OTest2 */

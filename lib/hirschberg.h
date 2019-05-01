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
#ifndef OTest2__LIB_HIRSCHBERG_H_
#define OTest2__LIB_HIRSCHBERG_H_

#include <assert.h>
#include <cstdint>
#include <vector>

namespace OTest2 {

enum class DiffAction : uint8_t {
  SUBSTR = 0, /**< the item is the same */
  CHANGE,     /**< the item has changed */
  INSERT,     /**< the item has been inserted into the left sequence */
  DELETE,     /**< the item has been deleted from the right sequence */
};

/**
 * @brief A diff change record
 *
 * The structure contains a difference computed by the LCS algorithm.
 * The action defines what has happened. The indexes point the touched
 * item(s) in the source sequences. If the action is the CHANGE, both
 * indexes are filled. If the action is the INSERT only left index
 * is filled. If the action is the DELETE only right index is filled.
 */
struct DiffRecord {
    DiffAction action;
    int left_index;
    int right_index;
};

/**
 * @brief Ordinary scoring function - standard LCS algorithm
 */
template<typename Type_>
class DiffScoreLCS {
  public:
    int scoreSub(
        Type_ left_,
        Type_ right_,
        DiffAction action_,
        bool& same_) const {
      same_ = left_ == right_;
      return same_ ? 1 : -1;
    }

    int scoreDel(
        Type_ right_,
        DiffAction action_) const {
      return -1;
    }

    int scoreIns(
        Type_ left_,
        DiffAction action_) const {
      return -1;
    }

    bool betterScore(
        int score1_,
        int score2_) const {
      return score1_ > score2_;
    }
};

namespace Hirschberg {

struct ScoreRecord {
    DiffAction action;
    int score;
};

template<typename Type_, typename ScoreFce_>
ScoreRecord selectAction(
    const ScoreFce_& score_fce_,
    Type_ left_,
    Type_ right_,
    const ScoreRecord& sub_prev_,
    const ScoreRecord& del_prev_,
    const ScoreRecord& ins_prev_) {
  /* -- compute scores for subsequence, insertion and deletion */
  bool same_;
  int sub_score_(
      sub_prev_.score + score_fce_.scoreSub(left_, right_, sub_prev_.action, same_));
  int del_score_(
      del_prev_.score + score_fce_.scoreDel(right_, del_prev_.action));
  int ins_score_(
      ins_prev_.score + score_fce_.scoreIns(left_, ins_prev_.action));

  /* -- find the extreme */
  if(score_fce_.betterScore(del_score_, ins_score_)) {
    if(score_fce_.betterScore(del_score_, sub_score_)) {
      return {DiffAction::DELETE, del_score_};
    }
    else {
      if(same_)
        return {DiffAction::SUBSTR, sub_score_};
      else
        return {DiffAction::CHANGE, sub_score_};
    }
  }
  else {
    if(score_fce_.betterScore(ins_score_, sub_score_)) {
      return {DiffAction::INSERT, ins_score_};
    }
    else {
      if(same_)
        return {DiffAction::SUBSTR, sub_score_};
      else
        return {DiffAction::CHANGE, sub_score_};
    }
  }
}

template<typename Type_, typename ScoreFce_>
void diffScore(
    const ScoreFce_& score_fce_,
    const Type_ left_[],
    int left_begin_,
    int left_end_,
    const Type_ right_[],
    int right_begin_,
    int right_end_,
    int step_,
    std::vector<ScoreRecord>& score_) {
  assert(step_ == 1 || step_ == -1);

  /* -- initialize first line */
  const int left_len_(score_.size());
  score_[0] = {DiffAction::SUBSTR, 0};
  for(int i_(1); i_ < left_len_; ++i_) {
    score_[i_] = {
        DiffAction::INSERT,
        score_[i_ - 1].score + score_fce_.scoreIns(
            left_[left_begin_ + (i_ - 1) * step_],
            score_[i_ - 1].action)};
  }

  /* -- compute the scores */
  std::vector<ScoreRecord> next_line_(left_len_, {DiffAction::INSERT, 0});
  for(; right_begin_ != right_end_; right_begin_ += step_) {
    next_line_[0] = {
        DiffAction::DELETE,
        score_[0].score + score_fce_.scoreDel(
            right_[right_begin_],
            score_[0].action)};
    for(int i_(1); i_ < left_len_; ++i_) {
      next_line_[i_] = selectAction(
          score_fce_,
          left_[left_begin_ + (i_ - 1) * step_],
          right_[right_begin_],
          score_[i_ - 1],
          score_[i_],
          next_line_[i_ - 1]);
    }
    score_.swap(next_line_);
  }
}

template<typename Type_, typename ScoreFunction_>
void diff(
    const ScoreFunction_& score_fce_,
    const Type_ left_[],
    int left_begin_,
    int left_end_,
    const Type_ right_[],
    int right_begin_,
    int right_end_,
    std::vector<DiffRecord>& diff_log_) {
  assert(left_begin_ >= 0 && left_end_ >= 0);
  assert(right_begin_ >= 0 && right_end_ >= 0);
  assert(left_begin_ <= left_end_ && right_begin_ <= right_end_);

  /* -- left is empty, the right tail is deleted */
  if(left_begin_ == left_end_) {
    for(int i_(right_begin_); i_ < right_end_; ++i_)
      diff_log_.push_back({DiffAction::DELETE, -1, i_});
  }
  /* -- right is empty, the left tail must be inserted */
  else if(right_begin_ == right_end_) {
    for(int i_(left_begin_); i_ < left_end_; ++i_)
      diff_log_.push_back({DiffAction::INSERT, i_, -1});
  }
  /* -- only one line */
  else if(left_begin_ + 1 == left_end_) {
    ScoreRecord last_sub_{DiffAction::SUBSTR, 0};
    ScoreRecord last_del_{
        DiffAction::INSERT,
        score_fce_.scoreIns(left_[left_begin_], DiffAction::SUBSTR)};
    int index_(-1);
    DiffAction change_action_;
    for(int i_(right_begin_); i_ < right_end_; ++i_) {
      ScoreRecord last_ins_{
          DiffAction::DELETE,
          last_sub_.score + score_fce_.scoreDel(right_[i_], last_sub_.action)};
      last_del_ = selectAction(
          score_fce_,
          left_[left_begin_],
          right_[i_],
          last_sub_,
          last_del_,
          last_ins_);
      if(last_del_.action == DiffAction::CHANGE
         || last_del_.action == DiffAction::SUBSTR) {
        index_ = i_;
        change_action_ = last_del_.action;
      }
      last_sub_ = last_ins_;
    }
    for(int i_(right_begin_); i_ < right_end_; ++i_) {
      if(i_ != index_)
        diff_log_.push_back({DiffAction::DELETE, -1, i_});
      else if(change_action_ == DiffAction::CHANGE)
        diff_log_.push_back({DiffAction::CHANGE, left_begin_, i_});
    }
  }
  /* -- only one column */
  else if(right_begin_ + 1 == right_end_) {
    ScoreRecord last_sub_{DiffAction::SUBSTR, 0};
    ScoreRecord last_ins_{
        DiffAction::DELETE,
        score_fce_.scoreDel(right_[right_begin_], DiffAction::SUBSTR)};
    int index_(-1);
    DiffAction change_action_;
    for(int i_(left_begin_); i_ < left_end_; ++i_) {
      ScoreRecord last_del_{
          DiffAction::INSERT,
          last_sub_.score + score_fce_.scoreIns(left_[i_], last_sub_.action)};
      last_ins_ = selectAction(
          score_fce_,
          left_[i_],
          right_[right_begin_],
          last_sub_,
          last_del_,
          last_ins_);
      if(last_ins_.action == DiffAction::CHANGE
         || last_ins_.action == DiffAction::SUBSTR) {
        index_ = i_;
        change_action_ = last_ins_.action;
      }
      last_sub_ = last_del_;
    }
    for(int i_(left_begin_); i_ < left_end_; ++i_) {
      if(i_ != index_)
        diff_log_.push_back({DiffAction::INSERT, i_, -1});
      else if(change_action_ == DiffAction::CHANGE)
        diff_log_.push_back({DiffAction::CHANGE, i_, right_begin_});
    }
  }
  /* -- the algorithm core */
  else {
    const int rmid_((right_begin_ + right_end_) / 2);
    const int left_len_(left_end_ - left_begin_);
    int lmid_(0);

    {
      /* -- prefixes */
      std::vector<ScoreRecord> score_pre_(left_len_ + 1);
      diffScore(
          score_fce_,
          left_,
          left_begin_,
          left_end_,
          right_,
          right_begin_,
          rmid_,
          1,
          score_pre_);

      /* -- suffixes */
      std::vector<ScoreRecord> score_suf_(left_len_ + 1);
      diffScore(
          score_fce_,
          left_,
          left_end_ - 1,
          left_begin_ - 1,
          right_,
          right_end_ - 1,
          rmid_ - 1,
          -1,
          score_suf_);

      /* -- find maximal score */
      int score_max_(score_pre_[0].score + score_suf_[left_len_].score);
      for(int i_(1); i_ <= left_len_; ++i_) {
        const int score_(score_pre_[i_].score + score_suf_[left_len_ - i_].score);
        if(score_ > score_max_) {
          score_max_ = score_;
          lmid_ = i_;
        }
      }
    }

    /* -- recurrent invocation */
    diff(
        score_fce_,
        left_,
        left_begin_,
        left_begin_ + lmid_,
        right_,
        right_begin_,
        rmid_,
        diff_log_);
    diff(
        score_fce_,
        left_,
        left_begin_ + lmid_,
        left_end_,
        right_,
        rmid_,
        right_end_,
        diff_log_);
  }
}

} /* -- namespace Hirschberg */

/**
 * @brief Compute the diff algorithm for specified sequences
 *
 * @param[in] left_ left sequence
 * @param[in] left_len_ Length of the left sequence
 * @param[in] right_ right sequence
 * @param[in] right_len_ Length of the right sequence
 * @param[out] diff_log_ list of diff changes
 */
template<typename Type_>
void hirschbergDiff(
    const Type_ left_[],
    std::size_t left_len_,
    const Type_ right_[],
    std::size_t right_len_,
    std::vector<DiffRecord>& diff_log_) {
  std::vector<DiffRecord> diff_;
  /* -- I expect 1/4 changes of the average length */
  diff_.reserve((left_len_ + right_len_) / 8);

  /* -- do the job */
  DiffScoreLCS<const Type_> score_fce_;
  Hirschberg::diff(
      score_fce_, left_, 0, left_len_, right_, 0, right_len_, diff_);

  /* -- return the result */
  diff_log_.swap(diff_);
}

} /* -- namespace OTest2 */

#endif /* OTest2__LIB_HIRSCHBERG_H_ */

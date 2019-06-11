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

#ifndef OTest2__LIB_DIFFLOGARRAY_H_
#define OTest2__LIB_DIFFLOGARRAY_H_

#include "hirschberg.h"

#include <vector>

namespace OTest2 {

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
    DiffAction action;  /**< the change: CHANGE, INSERT, DELETE */
    int left_index;     /**< index in the left sequence (valid: CHANGE, INSERT) */
    int right_index;    /**< index in the rigth sequence (valid: CHANGE, DELETE) */
};

/**
 * @brief Ordered (indexes into the sequences) list of diff changes
 */
typedef std::vector<DiffRecord> DiffLogArray;

/**
 * @brief Builder of the diff array
 */
class DiffLogBuilderArray : public DiffLogBuilder {
  private:
    DiffLogArray* array;

  public:
    /**
     * @brief Ctor
     *
     * @param array_ The log array. The ownership is not taken!
     */
    explicit DiffLogBuilderArray(
        DiffLogArray* array_);

    /**
     * @brief Dtor
     */
    virtual ~DiffLogBuilderArray();

    /* -- avoid copying */
    DiffLogBuilderArray(
        const DiffLogBuilderArray&) = delete;
    DiffLogBuilderArray& operator =(
        const DiffLogBuilderArray&) = delete;

    /* -- diff log builder interface */
    virtual void addMatch(
        int left_index_,
        int right_index_);
    virtual void addChange(
        int left_index_,
        int right_index_);
    virtual void addDelete(
        int right_index_);
    virtual void addInsert(
        int left_index_);
};

/**
 * @brief Compute the diff algorithm for specified sequences
 *
 * @param[in] left_ left sequence
 * @param[in] left_len_ Length of the left sequence
 * @param[in] right_ right sequence
 * @param[in] right_len_ Length of the right sequence
 * @param[out] diff_log_ list of diff changes
 * @param[in] score_fce_ A scoring function
 */
template<typename Type_, typename ScoreFce_ = DiffScoreLCS<Type_> >
void hirschbergDiff(
    const Type_ left_[],
    std::size_t left_len_,
    const Type_ right_[],
    std::size_t right_len_,
    DiffLogArray& diff_log_,
    ScoreFce_ score_fce_ = ScoreFce_()) {
  /* -- I expect 1/4 changes of the average length */
  DiffLogArray diff_;
  diff_.reserve((left_len_ + right_len_) / 8);

  /* -- do the job */
  DiffLogBuilderArray builder_(&diff_);
  hirschbergDiff(left_, left_len_, right_, right_len_, builder_, score_fce_);

  /* -- return the result */
  diff_log_.swap(diff_);
}

} /* namespace OTest2 */

#endif /* OTest2__LIB_DIFFLOGARRAY_H_ */

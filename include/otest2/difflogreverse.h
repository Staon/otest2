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
#ifndef OTest2__LIB_DIFFLOGREVERSE_H_
#define OTest2__LIB_DIFFLOGREVERSE_H_

#include <otest2/difflogbuilder.h>

namespace OTest2 {

/**
 * @brief Simple decorator - inversion of operations
 */
class DiffLogBuilderReverse : public DiffLogBuilder {
  private:
    DiffLogBuilder* next;

  public:
    /**
     * @brief Ctor
     *
     * @param next_ The decorated builder. The ownership is not taken.
     */
    explicit DiffLogBuilderReverse(
        DiffLogBuilder* next_);

    /**
     * @brief Dtor
     */
    virtual ~DiffLogBuilderReverse();

    /* -- avoid copying */
    DiffLogBuilderReverse(
        const DiffLogBuilderReverse&) = delete;
    DiffLogBuilderReverse& operator =(
        const DiffLogBuilderReverse&) = delete;

    /* -- interface of the diff log builder */
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

} /* namespace OTest2 */

#endif /* OTest2__LIB_DIFFLOGREVERSE_H_ */

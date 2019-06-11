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

#ifndef OTest2__LIB_DIFFLOGBLOCK_H_
#define OTest2__LIB_DIFFLOGBLOCK_H_

#include <vector>

#include "hirschberg.h"

namespace OTest2 {

/**
 * @brief One block change
 */
struct DiffBlock {
    int left_begin;
    int left_end;
    int right_begin;
    int right_end;
};

/**
 * @brief Ordered (indexes into the sequences) list of diff changes
 */
typedef std::vector<DiffBlock> DiffLogBlocks;

/**
 * @brief Log builder making records of whole blocks of changes
 */
class DiffLogBuilderBlock : public DiffLogBuilder {
  private:
    DiffLogBlocks* blocks;
    bool opened_block;

    void openBlock();

  public:
    /**
     * @brief Ctor
     *
     * @param blocks_ Injected array of diff blocks
     */
    explicit DiffLogBuilderBlock(
        DiffLogBlocks* blocks_);

    /**
     * @brief Dtor
     */
    virtual ~DiffLogBuilderBlock();

    /* -- avoid copying */
    DiffLogBuilderBlock(
        const DiffLogBuilderBlock&) = delete;
    DiffLogBuilderBlock& operator =(
        const DiffLogBuilderBlock&) = delete;

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

} /* namespace OTest2 */

#endif /* OTest2__LIB_DIFFLOGBLOCK_H_ */

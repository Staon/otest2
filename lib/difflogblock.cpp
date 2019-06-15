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
#include <otest2/difflogblock.h>

#include <assert.h>

namespace OTest2 {

namespace {

void updateRecord(
    DiffBlock& block_,
    int DiffBlock::* begin_,
    int DiffBlock::* end_,
    int value_) {
  assert(block_.*end_ == value_);
  ++(block_.*end_);
}

} /* -- namespace */

DiffLogBuilderBlock::DiffLogBuilderBlock(
    DiffLogBlocks* blocks_) :
  blocks(blocks_),
  opened_block(false),
  curr_left(-1),
  curr_right(-1) {
  assert(blocks != nullptr);

}

DiffLogBuilderBlock::~DiffLogBuilderBlock() {

}

void DiffLogBuilderBlock::openBlock() {
  if(!opened_block) {
    blocks->push_back({
      curr_left + 1, curr_left + 1, curr_right + 1, curr_right + 1});
    opened_block = true;
  }
  assert(!blocks->empty());
}

void DiffLogBuilderBlock::addMatch(
    int left_index_,
    int right_index_) {
  curr_left = left_index_;
  curr_right = right_index_;
  /* -- a match closes currently opened block of changes */
  opened_block = false;
}

void DiffLogBuilderBlock::addChange(
    int left_index_,
    int right_index_) {
  openBlock();
  curr_left = left_index_;
  curr_right = right_index_;

  DiffBlock& block_(blocks->back());
  updateRecord(
      block_, &DiffBlock::left_begin, &DiffBlock::left_end, left_index_);
  updateRecord(
      block_, &DiffBlock::right_begin, &DiffBlock::right_end, right_index_);
}

void DiffLogBuilderBlock::addDelete(
    int right_index_) {
  openBlock();
  curr_right = right_index_;

  DiffBlock& block_(blocks->back());
  updateRecord(
      block_, &DiffBlock::right_begin, &DiffBlock::right_end, right_index_);
}

void DiffLogBuilderBlock::addInsert(
    int left_index_) {
  openBlock();
  curr_left = left_index_;

  DiffBlock& block_(blocks->back());
  updateRecord(
      block_, &DiffBlock::left_begin, &DiffBlock::left_end, left_index_);
}

} /* namespace OTest2 */

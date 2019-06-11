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
#include "difflogreverse.h"

#include <assert.h>

namespace OTest2 {

DiffLogBuilderReverse::DiffLogBuilderReverse(
    DiffLogBuilder* next_) :
  next(next_) {
  assert(next != nullptr);

}

DiffLogBuilderReverse::~DiffLogBuilderReverse() {

}

void DiffLogBuilderReverse::addMatch(
    int left_index_,
    int right_index_) {
  next->addMatch(right_index_, left_index_);
}

void DiffLogBuilderReverse::addChange(
    int left_index_,
    int right_index_) {
  next->addChange(right_index_, left_index_);
}

void DiffLogBuilderReverse::addDelete(
    int right_index_) {
  next->addInsert(right_index_);
}

void DiffLogBuilderReverse::addInsert(
    int left_index_) {
  next->addDelete(left_index_);
}

} /* namespace OTest2 */

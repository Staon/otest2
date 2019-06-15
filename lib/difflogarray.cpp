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
#include <otest2/difflogarray.h>

#include <assert.h>

namespace OTest2 {

DiffLogBuilderArray::DiffLogBuilderArray(
    DiffLogArray* array_) :
  array(array_) {
  assert(array != nullptr);

}

DiffLogBuilderArray::~DiffLogBuilderArray() {

}

void DiffLogBuilderArray::addMatch(
    int left_index_,
    int right_index_) {
  /* -- nothing to do */
}

void DiffLogBuilderArray::addChange(
    int left_index_,
    int right_index_) {
  array->push_back({DiffAction::CHANGE, left_index_, right_index_});
}

void DiffLogBuilderArray::addDelete(
    int right_index_) {
  array->push_back({DiffAction::DELETE, -1, right_index_});
}

void DiffLogBuilderArray::addInsert(
    int left_index_) {
  array->push_back({DiffAction::INSERT, left_index_, -1});
}

} /* namespace OTest2 */

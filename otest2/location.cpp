/*
 * Copyright (C) 2018 Ondrej Starek
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

#include "location.h"

#include <algorithm>
#include <iostream>

namespace OTest2 {

Location::Location() :
  line(1),
  column(1) {

}

Location::Location(
    int line_,
    int column_) :
  line(line_),
  column(column_) {

}

Location::Location(
    const Location& loc_) :
  line(loc_.line),
  column(loc_.column) {

}

Location::~Location() {

}

void Location::swap(
    Location& loc_) noexcept {
  std::swap(line, loc_.line);
  std::swap(column, loc_.column);
}

Location& Location::operator = (
    const Location& loc_) {
  Location tmp_(loc_);
  swap(tmp_);
  return *this;
}

int Location::getLine() const {
  return line;
}

int Location::getColumn() const {
  return column;
}

int Location::compare(
    const Location& l2_) const {
  if(line < l2_.line)
    return -1;
  if(line > l2_.line)
    return 1;
  if(column < l2_.column)
    return -1;
  if(column > l2_.column)
    return 1;
  return 0;
}

void Location::incChar() {
  ++column;
}

void Location::nextLine() {
  ++line;
  column = 1;
}

std::ostream& Location::printObject(
    std::ostream& os_) const {
  return os_ << '[' << line << ':' << column << ']';
}

std::ostream& operator << (
    std::ostream& os_,
    const Location& loc_) {
  return loc_.printObject(os_);
}

bool operator < (
    const Location& l1_,
    const Location& l2_) {
  return l1_.compare(l2_) == -1;
}

bool operator <= (
    const Location& l1_,
    const Location& l2_) {
  return l1_.compare(l2_) != 1;
}

bool operator > (
    const Location& l1_,
    const Location& l2_) {
  return l1_.compare(l2_) == 1;
}

bool operator >= (
    const Location& l1_,
    const Location& l2_) {
  return l1_.compare(l2_) != -1;
}

bool operator == (
    const Location& l1_,
    const Location& l2_) {
  return l1_.compare(l2_) == 0;
}

bool operator != (
    const Location& l1_,
    const Location& l2_) {
  return l1_.compare(l2_) != 0;
}

} /* namespace OTest2 */

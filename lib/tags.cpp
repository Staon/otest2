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

#include <tags.h>

#include <algorithm>
#include <utility>

namespace OTest2 {

Tags::Tags() :
  tags() {

}

Tags::Tags(
    std::initializer_list<std::string> initializer_) :
  tags(initializer_) {

}

Tags::Tags(
    const Tags& other_) :
  tags(other_.tags) {

}

Tags::Tags(
    Tags&& other_) :
  tags(std::move(other_.tags)) {

}

Tags::~Tags() {

}

void Tags::swap(
    Tags& other_) noexcept {
  tags.swap(other_.tags);
}

Tags& Tags::operator = (
    const Tags& other_) {
  Tags tmp_(other_);
  swap(tmp_);
  return *this;
}

Tags& Tags::operator = (
    Tags&& other_) noexcept {
  swap(other_);
  return *this;
}

void Tags::appendTag(
    const std::string& tag_) {
  tags.push_back(tag_);
}

bool Tags::findTag(
    const std::string& tag_) const noexcept {
  /* -- there should be maximally ones of tags -> there is no need
   *    to implement better searching. */
  auto iter_(std::find(tags.begin(), tags.end(), tag_));
  return iter_ != tags.end();
}

bool Tags::isEmpty() const noexcept {
  return tags.empty();
}

} /* -- namespace OTest2 */

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
#include <tagsstack.h>

#include <algorithm>
#include <assert.h>
#include <iterator>
#include <vector>

#include <tags.h>
#include <utils.h>

namespace OTest2 {

struct TagsStack::Impl {
    std::vector<Tags> stack;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    Impl();
    ~Impl();
};

TagsStack::Impl::Impl() {

}

TagsStack::Impl::~Impl() {

}

TagsStack::TagsStack() :
  pimpl(new Impl) {

}

TagsStack::~TagsStack() {
  odelete(pimpl);
}

void TagsStack::pushTags(
    const Tags& tags_) {
  pimpl->stack.push_back(tags_);
}

void TagsStack::popTags() {
  assert(!pimpl->stack.empty());
  pimpl->stack.pop_back();
}

void TagsStack::fillTags(
    std::vector<Tags>& tags_) const {
  std::copy(pimpl->stack.begin(), pimpl->stack.end(), std::back_inserter(tags_));
}

} /* -- namespace OTest2 */

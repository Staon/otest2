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

#include <semanticstack.h>

#include <assert.h>
#include <vector>

#include <utils.h>

namespace OTest2 {

struct SemanticStack::Impl {
    SemanticStack* owner;

    typedef std::vector<bool> Stack;
    Stack stack;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        SemanticStack* owner_);
    ~Impl();
};

SemanticStack::Impl::Impl(
    SemanticStack* owner_) :
  owner(owner_) {

}

SemanticStack::Impl::~Impl() {

}

SemanticStack::SemanticStack() :
  pimpl(new Impl(this)) {

}

SemanticStack::~SemanticStack() {
  odelete(pimpl);
}

void SemanticStack::push(
    bool value_) {
  pimpl->stack.push_back(value_);
}

bool SemanticStack::top() const {
  assert(!pimpl->stack.empty());
  return pimpl->stack.back();
}

void SemanticStack::setTop(
    bool value_) {
  assert(!pimpl->stack.empty());
  pimpl->stack.back() = value_;
}

void SemanticStack::pop() {
  assert(!pimpl->stack.empty());
  pimpl->stack.pop_back();
}

void SemanticStack::popAnd() {
  assert(pimpl->stack.size() >= 2);
  bool value_(pimpl->stack.back());
  pimpl->stack.pop_back();
  pimpl->stack.back() = pimpl->stack.back() && value_;
}

bool SemanticStack::isFinished() const {
  return pimpl->stack.size() == 1;
}

} /* namespace OTest2 */

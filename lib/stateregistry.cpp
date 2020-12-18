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

#include <stateregistry.h>

#include <assert.h>
#include <map>

#include <utils.h>

namespace OTest2 {

struct StateRegistry::Impl {
  public:
    StateRegistry* owner;

    StatePtr first_state;
    typedef std::map<std::string, StatePtr> States;
    States states;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        StateRegistry* owner_);
    ~Impl();
};

StateRegistry::Impl::Impl(
    StateRegistry* owner_) :
  owner(owner_),
  first_state(),
  states() {

}

StateRegistry::Impl::~Impl() {

}

StateRegistry::StateRegistry() :
  pimpl(new Impl(this)) {

}

StateRegistry::~StateRegistry() {
  odelete(pimpl);
}

void StateRegistry::registerState(
    const std::string& name_,
    StatePtr state_) {
  assert(!name_.empty() && state_ != nullptr);
  pimpl->states.insert(Impl::States::value_type(name_, state_));
  if(pimpl->first_state == nullptr)
    pimpl->first_state = state_;
}

StatePtr StateRegistry::getFirstState() const {
  return pimpl->first_state;
}

StatePtr StateRegistry::getState(
    const std::string& name_) const {
  assert(!name_.empty());
  auto iter_(pimpl->states.find(name_));
  if(iter_ != pimpl->states.end())
    return (*iter_).second;
  else
    return StatePtr();
}

} /* namespace OTest2 */

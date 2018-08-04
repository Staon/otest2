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

#include <casegenerated.h>

#include <string>

#include <context.h>
#include "runcode.h"
#include <stateptr.h>
#include <stateregistry.h>
#include <utils.h>

namespace OTest2 {

struct CaseGenerated::Impl {
  public:
    CaseGenerated* owner;

    const Context* context;
    std::string name;
    StateRegistry state_registry;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        CaseGenerated* owner_,
        const Context& context_,
        const std::string& name_);
    ~Impl();
};

CaseGenerated::Impl::Impl(
    CaseGenerated* owner_,
    const Context& context_,
    const std::string& name_) :
  owner(owner_),
  context(&context_),
  name(name_),
  state_registry() {

}

CaseGenerated::Impl::~Impl() {

}

CaseGenerated::CaseGenerated(
    const Context& context_,
    const std::string& name_) :
  CaseOrdinary(context_),
  pimpl(new Impl(this, context_, name_)) {

}

CaseGenerated::~CaseGenerated() {
  odelete(pimpl);
}

std::string CaseGenerated::getName() const {
  return pimpl->name;
}

StatePtr CaseGenerated::getFirstState() const {
  return pimpl->state_registry.getFirstState();
}

void CaseGenerated::startUpCase(
    const Context& context_) {
  runUserCode(context_, [this](const Context& context_) {
    startUp();
  });
}

void CaseGenerated::tearDownCase(
    const Context& context_) {
  runUserCode(context_, [this](const Context& context_) {
    tearDown();
  });
}

const Context& CaseGenerated::otest2Context() const {
  return *pimpl->context;
}

void CaseGenerated::registerState(
    const std::string& name_,
    StatePtr state_) {
  pimpl->state_registry.registerState(name_, state_);
}

} /* -- namespace OTest2 */

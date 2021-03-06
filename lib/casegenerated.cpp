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

#include <assert.h>
#include <memory>
#include <string>
#include <vector>

#include <cmdfirststate.h>
#include <commandstack.h>
#include <context.h>
#include <fcemarshaler.h>
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
    std::string entering_state;
    std::vector<FceMarshalerPtr> start_ups;
    std::vector<FceMarshalerPtr> tear_downs;

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
  state_registry(),
  entering_state(),
  start_ups(),
  tear_downs() {

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

bool CaseGenerated::startUpObject(
    const Context& context_,
    int index_) {
  if(index_ >= 0 && index_ < pimpl->start_ups.size()) {
    runUserCode(context_, [this, index_](const Context& context_) {
      pimpl->start_ups[index_]->runFunction(context_);
    });
    return true;
  }
  else
    return false;
}

void CaseGenerated::scheduleBody(
    const Context& context_,
    ScenarioPtr scenario_,
    ObjectPtr me_) {
  assert(me_.get() == this);

  context_.command_stack->pushCommand(
      std::make_shared<CmdFirstState>(
          std::static_pointer_cast<CaseOrdinary>(me_)));
}

void CaseGenerated::tearDownObject(
    const Context& context_,
    int index_) {
  assert(index_ >= 0 && index_ < pimpl->tear_downs.size());

  runUserCode(context_, [this, index_](const Context& context_) {
    pimpl->tear_downs[index_]->runFunction(context_);
  });
}

StatePtr CaseGenerated::getFirstState() const {
  return pimpl->state_registry.getState(pimpl->entering_state);
}

StatePtr CaseGenerated::getState(
    const std::string& name_) const {
  return pimpl->state_registry.getState(name_);
}

const Context& CaseGenerated::otest2Context() const {
  return *pimpl->context;
}

void CaseGenerated::registerState(
    const std::string& name_,
    StatePtr state_) {
  pimpl->state_registry.registerState(name_, state_);
}

void CaseGenerated::setEnteringState(
    const std::string& name_) {
  pimpl->entering_state = name_;
}

void CaseGenerated::registerFixture(
    FceMarshalerPtr start_up_,
    FceMarshalerPtr tear_down_) {
  assert(start_up_ != nullptr && tear_down_ != nullptr);

  pimpl->start_ups.push_back(start_up_);
  pimpl->tear_downs.push_back(tear_down_);
}

} /* -- namespace OTest2 */

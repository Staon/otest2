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

#include <stategenerated.h>

#include <assert.h>
#include <string>

#include <caseordinaryptr.h>
#include <cmdstate.h>
#include <commandstack.h>
#include <context.h>
#include <objectpath.h>
#include "runcode.h"
#include <utils.h>

namespace OTest2 {

struct StateGenerated::Impl {
  public:
    StateGenerated* owner;

    const Context* context;
    std::string name;
    ObjectPath section;

    CaseOrdinaryPtr parent;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        StateGenerated* owner_,
        const Context& context_,
        const std::string& name_,
        const std::string& section_path_);
    ~Impl();
};

StateGenerated::Impl::Impl(
    StateGenerated* owner_,
    const Context& context_,
    const std::string& name_,
    const std::string& section_path_) :
  owner(owner_),
  context(&context_),
  name(name_),
  section(section_path_),
  parent() {

}

StateGenerated::Impl::~Impl() {

}

StateGenerated::StateGenerated(
    const Context& context_,
    const std::string& name_,
    const std::string& section_path_) :
  StateOrdinary(context_),
  pimpl(new Impl(this, context_, name_, section_path_)) {

}

StateGenerated::~StateGenerated() {
  odelete(pimpl);
}

std::string StateGenerated::getName() const {
  return pimpl->name;
}

void StateGenerated::executeState(
    const Context& context_,
    CaseOrdinaryPtr parent_) {
  assert(parent_ != nullptr);

  pimpl->parent = parent_;
  runUserCode(context_, [this](const Context& context_) {
    runState(context_);
  });
  pimpl->parent = CaseOrdinaryPtr();
}

const Context& StateGenerated::otest2Context() const {
  return *pimpl->context;
}

void StateGenerated::switchState(
    const Context& context_,
    const std::string& name_,
    int delay_) {
  assert(pimpl->parent != nullptr && !name_.empty() && delay_ >= 0);

  /* -- schedule the commands */
  context_.command_stack->replaceCommand(
      std::make_shared<CmdState>(pimpl->parent, name_, delay_));
}

bool StateGenerated::isTestSectionActive(
    const Context& context_,
    const std::string& section_path_) {
  const ObjectPath path_(section_path_);
  return path_.isPrefixOf(pimpl->section);
}

} /* -- namespace OTest2 */

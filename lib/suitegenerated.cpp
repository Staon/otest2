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

#include <suitegenerated.h>

#include <string>

#include <caseregistry.h>
#include <context.h>
#include "runcode.h"
#include <utils.h>

namespace OTest2 {

struct SuiteGenerated::Impl {
  public:
    SuiteGenerated* owner;

    const Context* context;
    std::string name;
    CaseRegistry case_registry;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        SuiteGenerated* owner_,
        const Context& context_,
        const std::string& name_);
    ~Impl();
};

SuiteGenerated::Impl::Impl(
    SuiteGenerated* owner_,
    const Context& context_,
    const std::string& name_) :
  owner(owner_),
  context(&context_),
  name(name_),
  case_registry() {

}

SuiteGenerated::Impl::~Impl() {

}

SuiteGenerated::SuiteGenerated(
    const Context& context_,
    const std::string& name_) :
  SuiteOrdinary(context_),
  pimpl(new Impl(this, context_, name_)) {

}

SuiteGenerated::~SuiteGenerated() {
  odelete(pimpl);
}

std::string SuiteGenerated::getName() const {
  return pimpl->name;
}

const Context& SuiteGenerated::otest2Context() const {
  return *pimpl->context;
}

CaseFactoryPtr SuiteGenerated::getCase(
    const Context& context_,
    int index_) const {
  return pimpl->case_registry.getCase(index_);
}

void SuiteGenerated::startUpSuite(
    const Context& context_) {
  runUserCode(context_, [this](const Context& context_) {
    startUp();
  });
}

void SuiteGenerated::tearDownSuite(
    const Context& context_) {
  runUserCode(context_, [this](const Context& context_){
    tearDown();
  });
}

void SuiteGenerated::registerCase(
    const std::string& name_,
    CaseFactoryPtr case_factory_) {
  pimpl->case_registry.registerCase(name_, case_factory_);
}

} /* -- namespace OTest2 */

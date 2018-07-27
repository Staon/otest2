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

#include <caseregistry.h>

#include <assert.h>
#include <map>

namespace OTest2 {

struct CaseRegistry::Impl {
  public:
    CaseRegistry* owner;

    typedef std::map<std::string, CaseFactoryPtr> CaseRegistryMap;
    CaseRegistryMap registry;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        CaseRegistry* owner_);
    ~Impl();
};

CaseRegistry::Impl::Impl(
    CaseRegistry* owner_) :
  owner(owner_) {

}

CaseRegistry::Impl::~Impl() {

}

CaseRegistry::CaseRegistry() :
  pimpl(new Impl(this)) {

}

CaseRegistry::~CaseRegistry() {
  delete pimpl;
  pimpl = nullptr;
}

void CaseRegistry::registerCase(
    const std::string& name_,
    CaseFactoryPtr case_factory_) {
  assert(!name_.empty() && case_factory_ != nullptr);
  pimpl->registry.insert(
      Impl::CaseRegistryMap::value_type(name_, case_factory_));
}

} /* namespace OTest2 */

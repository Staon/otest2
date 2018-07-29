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
#include <vector>

#include <utils.h>

namespace OTest2 {

struct CaseRegistry::Impl {
  public:
    CaseRegistry* owner;

    typedef std::map<std::string, CaseFactoryPtr> CaseRegistryMap;
    CaseRegistryMap registry;
    typedef std::vector<CaseFactoryPtr> Order;
    Order order;

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
  odelete(pimpl);
}

void CaseRegistry::registerCase(
    const std::string& name_,
    CaseFactoryPtr case_factory_) {
  assert(!name_.empty() && case_factory_ != nullptr);
  auto result_(pimpl->registry.insert(
      Impl::CaseRegistryMap::value_type(name_, case_factory_)));
  if(result_.second)
    pimpl->order.push_back(case_factory_);
}

CaseFactoryPtr CaseRegistry::getCase(
    int index_) const {
  if(index_ >= 0 && static_cast<Impl::Order::size_type>(index_) < pimpl->order.size())
    return pimpl->order[index_];
  else
    return nullptr;
}

} /* namespace OTest2 */

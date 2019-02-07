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

#include <otest2/registry.h>

#include <assert.h>
#include <map>
#include <vector>

#include <utils.h>

namespace OTest2 {

struct Registry::Impl {
  public:
    Registry* owner;

    typedef std::map<std::string, int> SuiteRegistry;
    SuiteRegistry registry;
    typedef std::vector<std::pair<std::string, SuiteFactoryPtr> > Order;
    Order order;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        Registry* owner_);
    ~Impl();
};

Registry::Impl::Impl(
    Registry* owner_) :
  owner(owner_) {

}

Registry::Impl::~Impl() {

}

Registry::Registry() :
  pimpl(new Impl(this)) {

}

Registry::~Registry() {
  odelete(pimpl);
}

void Registry::registerSuite(
    const std::string& name_,
    SuiteFactoryPtr suite_factory_) {
  assert(!name_.empty() && suite_factory_ != nullptr);
  auto result_(pimpl->registry.insert(
      Impl::SuiteRegistry::value_type(name_, pimpl->order.size())));
  if(result_.second)
    pimpl->order.push_back(Impl::Order::value_type(name_, suite_factory_));
}

SuiteFactoryPtr Registry::getSuite(
    int index_,
    std::string* name_) const {
  if(index_ >= 0
     && static_cast<Impl::Order::size_type>(index_) < pimpl->order.size()) {
    if(name_ != nullptr)
      *name_ = pimpl->order[index_].first;
    return pimpl->order[index_].second;
  }
  else {
    return nullptr;
  }
}

Registry& Registry::instance(
    const std::string& domain_) {
  typedef std::map<std::string, Registry> TestDomains;
  static TestDomains domains_;
  auto iter_(domains_.find(domain_));
  if(iter_ == domains_.end()) {
    auto insert_ret_(domains_.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(domain_),
        std::forward_as_tuple()));
    assert(insert_ret_.second);
    iter_ = insert_ret_.first;
  }
  return (*iter_).second;
}

} /* namespace OTest2 */

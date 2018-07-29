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

    typedef std::map<std::string, SuiteFactoryPtr> SuiteRegistry;
    SuiteRegistry registry;
    typedef std::vector<SuiteFactoryPtr> Order;
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
      Impl::SuiteRegistry::value_type(name_, suite_factory_)));
  if(result_.second)
    pimpl->order.push_back(suite_factory_);
}

SuiteFactoryPtr Registry::getSuite(
    int index_) {
  if(index_ >= 0
     && static_cast<Impl::Order::size_type>(index_) < pimpl->order.size()) {
    return pimpl->order[index_];
  }
  else {
    return nullptr;
  }
}

Registry& Registry::instance() {
  static Registry registry_;
  return registry_;
}

} /* namespace OTest2 */

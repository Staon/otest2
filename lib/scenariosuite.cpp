/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2
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

#include <scenariosuite.h>

#include <assert.h>
#include <map>
#include <memory>
#include <vector>

#include <context.h>
#include <objectpath.h>
#include <objectrepeater.h>
#include <objectrepeaterfactory.h>
#include <reporter.h>
#include "scenarioitercontainer.h"
#include <utils.h>

namespace OTest2 {

struct ScenarioSuite::Impl {
    std::string name;
    ObjectRepeaterFactoryPtr repeater_factory;
    typedef std::map<std::string, ScenarioPtr> Children;
    Children children;
    typedef std::vector<Children::iterator> Order;
    Order order;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& name_,
        ObjectRepeaterFactoryPtr repeater_factory_);
    ~Impl();
};

ScenarioSuite::Impl::Impl(
    const std::string& name_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  name(name_),
  repeater_factory(repeater_factory_),
  children() {
  assert(!name_.empty() && repeater_factory != nullptr);

}

ScenarioSuite::Impl::~Impl() {

}

ScenarioSuite::ScenarioSuite(
    const std::string& name_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  pimpl(new Impl(name_, repeater_factory_)) {

}

ScenarioSuite::~ScenarioSuite() {
  odelete(pimpl);
}

ScenarioPtr ScenarioSuite::filterScenario(
    ObjectPath& path_,
    TagsStack& tags_,
    ScenarioContainerPtr parent_,
    const RunnerFilter& name_filter_,
    const TagFilter& tag_filter_) const {
  /* -- add myself into the object path */
  path_.pushName(pimpl->name);

  /* -- create new suite container and filter children */
  ScenarioContainerPtr suite_(std::make_shared<ScenarioSuite>(
      pimpl->name, pimpl->repeater_factory));
  for(auto iter_ : pimpl->order) {
    (*iter_).second->filterScenario(
        path_, tags_, suite_, name_filter_, tag_filter_);
  }

  /* -- append myself if there are some not-filtered children */
  if(!suite_->isEmpty()) {
    parent_->appendScenario(pimpl->name, suite_);
  }

  /* -- remove myself from the object path */
  path_.popName();

  return parent_;
}

std::pair<std::string, ObjectRepeaterPtr> ScenarioSuite::createRepeater(
    const Context& context_) const {
  return {pimpl->name, pimpl->repeater_factory->createRepeater(context_)};
}

void ScenarioSuite::reportEntering(
    const Context& context_,
    const std::string& decorated_name_) const noexcept {
  context_.reporter->enterSuite(context_, decorated_name_);
}

void ScenarioSuite::reportLeaving(
    const Context& context_,
    const std::string& decorated_name_,
    bool result_) const noexcept {
  context_.reporter->leaveSuite(context_, decorated_name_, result_);
}

ScenarioIterPtr ScenarioSuite::getChildren() const {
  std::vector<ScenarioPtr> items_;
  items_.reserve(pimpl->children.size());
  for(auto iter_ : pimpl->order) {
    items_.push_back((*iter_).second);
  }
  return std::make_shared<ScenarioIterContainer>(items_);
}

void ScenarioSuite::appendScenario(
    const std::string& name_,
    ScenarioPtr scenario_) {
  assert(!name_.empty() && scenario_ != nullptr);
  auto item_(pimpl->children.insert(Impl::Children::value_type(name_, scenario_)));
  if(item_.second) {
    pimpl->order.push_back(item_.first);
  }
  else {
    assert(false);
  }
}

bool ScenarioSuite::isEmpty() const noexcept {
  return pimpl->children.empty();
}

} /* -- namespace OTest2 */

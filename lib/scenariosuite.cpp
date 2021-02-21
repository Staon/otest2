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
#include <semanticstack.h>
#include <tags.h>
#include <tagsstack.h>
#include <utils.h>

namespace OTest2 {

struct ScenarioSuite::Impl {
    std::string name;
    Tags tags;
    ObjectRepeaterFactoryPtr repeater_factory;
    typedef std::vector<ScenarioPtr> Children;
    Children children;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& name_,
        const Tags& tags_,
        ObjectRepeaterFactoryPtr repeater_factory_);
    ~Impl();
};

ScenarioSuite::Impl::Impl(
    const std::string& name_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  name(name_),
  tags(tags_),
  repeater_factory(repeater_factory_),
  children() {
  assert(!name_.empty() && repeater_factory != nullptr);

}

ScenarioSuite::Impl::~Impl() {

}

ScenarioSuite::ScenarioSuite(
    const std::string& name_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  pimpl(new Impl(name_, tags_, repeater_factory_)) {

}

ScenarioSuite::~ScenarioSuite() {
  odelete(pimpl);
}

ScenarioPtr ScenarioSuite::filterScenario(
    TagsStack& tags_,
    ScenarioContainerPtr parent_,
    const RunnerFilter& filter_) const {
  /* -- add myself into the object path */
  tags_.pushTags(pimpl->name, pimpl->tags);

  /* -- create new suite container and filter children */
  ScenarioContainerPtr suite_(std::make_shared<ScenarioSuite>(
      pimpl->name, pimpl->tags, pimpl->repeater_factory));
  for(auto iter_ : pimpl->children) {
    iter_->filterScenario(tags_, suite_, filter_);
  }

  /* -- append myself if there are some not-filtered children */
  if(!suite_->isEmpty()) {
    parent_->appendScenario(suite_);
  }

  /* -- remove myself from the object path */
  tags_.popTags();

  return parent_;
}

std::pair<std::string, ObjectRepeaterPtr> ScenarioSuite::createRepeater(
    const Context& context_) const {
  return {pimpl->name, pimpl->repeater_factory->createRepeater(context_, std::string())};
}

void ScenarioSuite::enterObject(
    const Context& context_) const noexcept {
  context_.reporter->enterSuite(
      context_,
      context_.object_path->getCurrentName(),
      context_.object_path->getCurrentParameters());
}

void ScenarioSuite::leaveObject(
    const Context& context_) const noexcept {
  context_.reporter->leaveSuite(
      context_,
      context_.object_path->getCurrentName(),
      context_.object_path->getCurrentParameters(),
      context_.semantic_stack->top());
}

ScenarioIterPtr ScenarioSuite::getChildren() const {
  return std::make_shared<ScenarioIterContainer>(pimpl->children);
}

void ScenarioSuite::appendScenario(
    ScenarioPtr scenario_) {
  assert(scenario_ != nullptr);
  pimpl->children.push_back(scenario_);
}

bool ScenarioSuite::isEmpty() const noexcept {
  return pimpl->children.empty();
}

} /* -- namespace OTest2 */

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
#include <otest2/scenarioroot.h>

#include <assert.h>
#include <map>
#include <memory>
#include <string>

#include <context.h>
#include <objectpath.h>
#include <objectrepeateronce.h>
#include <reporter.h>
#include "scenarioitercontainer.h"
#include <scenariocontainerptr.h>
#include <scenarioptr.h>
#include <semanticstack.h>
#include <testroot.h>
#include <utils.h>

namespace OTest2 {

namespace {

class ObjectRepeaterRoot : public ObjectRepeaterOnce {
  private:
    virtual ObjectScenarioPtr doCreateObject(
        const Context& context_,
        const std::string& decorated_name_,
        ObjectPtr parent_) override;

  public:
    ObjectRepeaterRoot() = default;
    virtual ~ObjectRepeaterRoot() = default;

    /* -- avoid copying */
    ObjectRepeaterRoot(
        const ObjectRepeaterRoot&) = delete;
    ObjectRepeaterRoot& operator = (
        const ObjectRepeaterRoot&) = delete;
};

ObjectScenarioPtr ObjectRepeaterRoot::doCreateObject(
    const Context& context_,
    const std::string& decorated_name_,
    ObjectPtr parent_) {
  return std::make_shared<TestRoot>(decorated_name_);
}

} /* -- namespace */

struct ScenarioRoot::Impl {
    std::string name;
    typedef std::vector<ScenarioPtr> Children;
    Children children;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& name_);
    ~Impl();
};

ScenarioRoot::Impl::Impl(
    const std::string& name_) :
  name(name_),
  children() {
  assert(!name.empty());

}

ScenarioRoot::Impl::~Impl() {

}

ScenarioRoot::ScenarioRoot(
    const std::string& name_) :
  pimpl(new Impl(name_)) {

}

ScenarioRoot::~ScenarioRoot() {
  odelete(pimpl);
}

void ScenarioRoot::setName(
    const std::string& name_) {
  assert(!name_.empty());
  pimpl->name = name_;
}

ScenarioPtr ScenarioRoot::filterScenario(
    TagsStack& tags_,
    ScenarioContainerPtr parent_,
    const RunnerFilter& filter_) const {
  ScenarioContainerPtr root_(std::make_shared<ScenarioRoot>(pimpl->name));
  for(auto item_ : pimpl->children) {
    item_->filterScenario(tags_, root_, filter_);
  }

  return root_;
}

std::pair<std::string, ObjectRepeaterPtr> ScenarioRoot::createRepeater(
    const Context& context_) const {
  return {pimpl->name, std::make_shared<ObjectRepeaterRoot>()};
}

void ScenarioRoot::enterObject(
    const Context& context_) const noexcept {
  context_.reporter->enterTest(
      context_,
      context_.object_path->getCurrentName(),
      context_.object_path->getCurrentParameters());
}

void ScenarioRoot::leaveObject(
    const Context& context_) const noexcept {
  context_.reporter->leaveTest(
      context_,
      context_.object_path->getCurrentName(),
      context_.object_path->getCurrentParameters(),
      context_.semantic_stack->top());
}

ScenarioIterPtr ScenarioRoot::getChildren() const {
  return std::make_shared<ScenarioIterContainer>(pimpl->children);
}

void ScenarioRoot::appendScenario(
    ScenarioPtr scenario_) {
  assert(scenario_ != nullptr);
  pimpl->children.push_back(scenario_);
}

bool ScenarioRoot::isEmpty() const noexcept {
  return pimpl->children.empty();
}

} /* -- namespace OTest2 */

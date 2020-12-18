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
#include <scenariocase.h>

#include <assert.h>

#include <context.h>
#include <objectpath.h>
#include <objectrepeater.h>
#include <objectrepeaterfactory.h>
#include <reporter.h>
#include <runnerfilter.h>
#include <scenariocontainer.h>
#include <tagfilter.h>
#include <tags.h>
#include <tagsstack.h>
#include <utils.h>

namespace OTest2 {

struct ScenarioCase::Impl {
    std::string name;
    Tags tags;
    ObjectRepeaterFactoryPtr repeater_factory;

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

ScenarioCase::Impl::Impl(
    const std::string& name_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  name(name_),
  tags(tags_),
  repeater_factory(repeater_factory_) {
  assert(!name.empty() && repeater_factory != nullptr);

}

ScenarioCase::Impl::~Impl() {

}

ScenarioCase::ScenarioCase(
    const std::string& name_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  pimpl(new Impl(name_, tags_, repeater_factory_)) {

}

ScenarioCase::~ScenarioCase() {
  odelete(pimpl);
}

ScenarioPtr ScenarioCase::filterScenario(
    ObjectPath& path_,
    TagsStack& tags_,
    ScenarioContainerPtr parent_,
    const RunnerFilter& name_filter_,
    const TagFilter& tag_filter_) const {
  /* -- add my name into the whole path */
  path_.pushName(pimpl->name);
  tags_.pushTags(pimpl->tags);

  /* -- if the object is not filtered append it into the parent container */
  if(!name_filter_.filterPath(path_) && !tag_filter_.filterObject(tags_)) {
    /* -- the object is supposed to be run add myself into the parent
     *    container. */
    parent_->appendScenario(
        pimpl->name,
        std::make_shared<ScenarioCase>(
            pimpl->name, pimpl->tags, pimpl->repeater_factory));
  }

  /* -- pop my name */
  path_.popName();
  tags_.popTags();

  return parent_;
}

std::pair<std::string, ObjectRepeaterPtr> ScenarioCase::createRepeater(
    const Context& context_) const {
  return {pimpl->name, pimpl->repeater_factory->createRepeater(context_)};
}

void ScenarioCase::reportEntering(
    const Context& context_,
    const std::string& decorated_name_) const noexcept {
  context_.reporter->enterCase(context_, decorated_name_);
}

void ScenarioCase::reportLeaving(
    const Context& context_,
    const std::string& decorated_name_,
    bool result_) const noexcept {
  context_.reporter->leaveCase(context_, decorated_name_, result_);
}

ScenarioIterPtr ScenarioCase::getChildren() const {
  assert(false);
  return ScenarioIterPtr();
}

} /* -- namespace OTest2 */

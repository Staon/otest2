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
#include <vector>

#include <context.h>
#include <objectpath.h>
#include <objectrepeater.h>
#include <objectrepeaterfactory.h>
#include <reporter.h>
#include <runnerfilter.h>
#include <scenariocontainer.h>
#include <tags.h>
#include <tagsstack.h>
#include <utils.h>

namespace OTest2 {


struct ScenarioCase::Impl {
    std::string name;
    std::string section_path;
    Tags tags;
    ObjectRepeaterFactoryPtr repeater_factory;

    /* -- registered test scenarios */
    struct Section {
        std::string name;
        std::vector<Section> children;

        void filterScenario(
            ScenarioCase::Impl* pimpl_,
            ObjectPath& path_,
            TagsStack& tags_,
            ScenarioContainerPtr parent_,
            const RunnerFilter& filter_) const;
    };
    std::vector<Section> sections;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& name_,
        const std::string& section_path_,
        const Tags& tags_,
        ObjectRepeaterFactoryPtr repeater_factory_);
    ~Impl();
};

ScenarioCase::Impl::Impl(
    const std::string& name_,
    const std::string& section_path_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  name(name_),
  section_path(section_path_),
  tags(tags_),
  repeater_factory(repeater_factory_),
  sections() {
  assert(!name.empty() && repeater_factory != nullptr);

}

ScenarioCase::Impl::~Impl() {

}

void ScenarioCase::Impl::Section::filterScenario(
    Impl* pimpl_,
    ObjectPath& path_,
    TagsStack& tags_,
    ScenarioContainerPtr parent_,
    const RunnerFilter& filter_) const {
  /* -- add my name and tags into the whole path */
  path_.pushName(name);
  tags_.pushTags(name, pimpl_->tags);

  if(children.empty()) {
    /* -- I am a leaf - check whether I match the filter */
    if(!filter_.filterPath(tags_)) {
      parent_->appendScenario(
          std::make_shared<ScenarioCase>(
              pimpl_->name, path_.getCurrentPath(), pimpl_->tags, pimpl_->repeater_factory));
    }
  }
  else {
    /* -- recursively iterate the children scenarios */
    for(const auto& scenario_ : children)
      scenario_.filterScenario(pimpl_, path_, tags_, parent_, filter_);
  }

  /* -- pop me */
  tags_.popTags();
  path_.popName();
}

ScenarioCase::ScenarioCase(
    const std::string& name_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  pimpl(new Impl(name_, std::string(), tags_, repeater_factory_)) {

}

ScenarioCase::ScenarioCase(
    const std::string& name_,
    const std::string& section_path_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  pimpl(new Impl(name_, section_path_, tags_, repeater_factory_)) {

}

ScenarioCaseBuilder ScenarioCase::createBuilder(
    const std::string& name_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) {
  return ScenarioCaseBuilder(name_, tags_, repeater_factory_);
}

ScenarioCase::~ScenarioCase() {
  odelete(pimpl);
}

ScenarioPtr ScenarioCase::filterScenario(
    TagsStack& tags_,
    ScenarioContainerPtr parent_,
    const RunnerFilter& filter_) const {
  /* -- add my name nad tags into the whole path */
  tags_.pushTags(pimpl->name, pimpl->tags);

  if(pimpl->sections.empty()) {
    /* -- there are no scenarios -> check whether I am filtered */
    if(!filter_.filterPath(tags_)) {
      parent_->appendScenario(
          std::make_shared<ScenarioCase>(
              pimpl->name, pimpl->tags, pimpl->repeater_factory));
    }
  }
  else {
    /* -- check the scenarios */
    ObjectPath path_;
    for(const auto& scenario : pimpl->sections)
      scenario.filterScenario(pimpl, path_, tags_, parent_, filter_);
  }

  /* -- pop me */
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

struct ScenarioCaseBuilder::Impl {
    ScenarioPtr scenario;
    ScenarioCase* scenario_case;
    std::vector<ScenarioCase::Impl::Section*> section_stack;

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

ScenarioCaseBuilder::Impl::Impl(
    const std::string& name_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  scenario(std::make_shared<ScenarioCase>(name_, tags_, repeater_factory_)),
  scenario_case(static_cast<ScenarioCase*>(scenario.get())),
  section_stack() {

}

ScenarioCaseBuilder::Impl::~Impl() {

}

ScenarioCaseBuilder::ScenarioCaseBuilder(
    const std::string& name_,
    const Tags& tags_,
    ObjectRepeaterFactoryPtr repeater_factory_) :
  pimpl(new Impl(name_, tags_, repeater_factory_)) {

}

ScenarioCaseBuilder::ScenarioCaseBuilder(
    ScenarioCaseBuilder&& other_) :
  pimpl(other_.pimpl) {
  other_.pimpl = nullptr;
}

ScenarioCaseBuilder::~ScenarioCaseBuilder() {
  odelete(pimpl);
}

ScenarioCaseBuilder& ScenarioCaseBuilder::pushSection(
    const std::string& name_) {
  assert(!name_.empty());

  if(pimpl->section_stack.empty()) {
    pimpl->scenario_case->pimpl->sections.push_back(ScenarioCase::Impl::Section{name_, {}});
    pimpl->section_stack.push_back(&pimpl->scenario_case->pimpl->sections.back());
  }
  else {
    auto top_(pimpl->section_stack.back());
    top_->children.push_back(ScenarioCase::Impl::Section{name_, {}});
    pimpl->section_stack.push_back(&top_->children.back());
  }

  return *this;
}

ScenarioCaseBuilder& ScenarioCaseBuilder::popSection() {
  assert(!pimpl->section_stack.empty());
  pimpl->section_stack.pop_back();

  return *this;
}

ScenarioPtr ScenarioCaseBuilder::getScenario() {
  return pimpl->scenario;
}

} /* -- namespace OTest2 */

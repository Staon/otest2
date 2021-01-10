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

#include <registry.h>

#include <assert.h>
#include <map>
#include <memory>
#include <vector>

#include <context.h>
#include <objectpath.h>
#include <objectrepeateronce.h>
#include <reporter.h>
#include <semanticstack.h>
#include <scenario.h>
#include <scenarioiter.h>
#include <scenarioroot.h>
#include <tagsstack.h>
#include <utils.h>

namespace OTest2 {

namespace {

class RootIter : public ScenarioIter {
  private:
    ScenarioPtr root;

  public:
    explicit RootIter(
        ScenarioPtr root_);
    virtual ~RootIter() = default;

    /* -- avoid copying */
    RootIter(
        const RootIter&) = delete;
    RootIter& operator = (
        const RootIter&) = delete;

    /* -- scenario iterator */
    virtual bool isValid() const noexcept override;
    virtual ScenarioPtr getScenario() const noexcept override;
    virtual void next() noexcept override;
};

RootIter::RootIter(
    ScenarioPtr root_) :
  root(root_) {
  assert(root != nullptr);

}

bool RootIter::isValid() const noexcept {
  return root != nullptr;
}

ScenarioPtr RootIter::getScenario() const noexcept {
  assert(root != nullptr);
  return root;
}

void RootIter::next() noexcept {
  root = nullptr;
}

} /* -- namespace */

struct Registry::Impl {
  public:
    Registry* owner;
    ScenarioContainerPtr scenario_root;

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
  owner(owner_),
  scenario_root(std::make_shared<ScenarioRoot>("test")) {

}

Registry::Impl::~Impl() {

}

Registry::Registry() :
  pimpl(new Impl(this)) {

}

Registry::~Registry() {
  odelete(pimpl);
}

void Registry::registerScenario(
    const std::string& name_,
    ScenarioPtr scenario_) {
  pimpl->scenario_root->appendScenario(name_, scenario_);
}

void Registry::setTestName(
    const std::string& name_) {
  assert(!name_.empty());
  std::static_pointer_cast<ScenarioRoot>(pimpl->scenario_root)->setName(name_);
}

ScenarioIterPtr Registry::getTests(
    const RunnerFilter& filter_) const {
  /* -- filter the scenario */
  TagsStack tags_stack_;
  ScenarioPtr filtered_root_(
      pimpl->scenario_root->filterScenario(tags_stack_, nullptr, filter_));

  /* -- return the iterator */
  return std::make_shared<RootIter>(filtered_root_);
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

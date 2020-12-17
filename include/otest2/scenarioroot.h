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

#ifndef OTest2_INCLUDE_OTEST2_SCENARIOROOT_H_
#define OTest2_INCLUDE_OTEST2_SCENARIOROOT_H_

#include <otest2/scenariocontainer.h>

namespace OTest2 {

/**
 * @brief Scenario object representing entire test
 */
class ScenarioRoot : public ScenarioContainer {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     *
     * @param name_ Name of the test
     */
    explicit ScenarioRoot(
        const std::string& name_);

    /**
     * @brief Dtor
     */
    virtual ~ScenarioRoot();

    /* -- avoid copying */
    ScenarioRoot(
        const ScenarioRoot&) = delete;
    ScenarioRoot& operator = (
        const ScenarioRoot&) = delete;

    /**
     * @brief Set name of the scenario (the name of the entire test)
     *
     * @param name_ The name
     */
    void setName(
        const std::string& name_);

    /* -- scenario interface */
    virtual ScenarioPtr filterScenario(
        ObjectPath& path_,
        TagsStack& tags_,
        ScenarioContainerPtr parent_,
        const RunnerFilter& name_filter_,
        const TagFilter& tag_filter_) const;
    virtual std::pair<std::string, ObjectRepeaterPtr> createRepeater(
        const Context& context_) const override;
    virtual void reportEntering(
        const Context& context_,
        const std::string& decorated_name_) const noexcept override;
    virtual void reportLeaving(
        const Context& context_,
        const std::string& decorated_name_,
        bool result_) const noexcept override;
    virtual ScenarioIterPtr getChildren() const override;

    /* -- scenario container interface */
    virtual void appendScenario(
        const std::string& name_,
        ScenarioPtr scenario_) override;
    virtual bool isEmpty() const noexcept override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_SCENARIOROOT_H_ */

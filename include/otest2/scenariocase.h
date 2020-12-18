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

#ifndef OTest2_INCLUDE_OTEST2_SCENARIOCASE_H_
#define OTest2_INCLUDE_OTEST2_SCENARIOCASE_H_

#include <otest2/objectrepeaterfactoryptr.h>
#include <otest2/scenario.h>

namespace OTest2 {

class Tags;

/**
 * @brief Scenario object of a test case
 */
class ScenarioCase : public Scenario {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     *
     * @param name_ Name of the test case
     * @param tags_ Tags assigned to the object
     * @param repeater_factory A factory of repeater objects of the test case
     */
    explicit ScenarioCase(
        const std::string& name_,
        const Tags& tags_,
        ObjectRepeaterFactoryPtr repeater_factory_);

    /**
     * @brief Dtor
     */
    virtual ~ScenarioCase();

    /* -- avoid copying */
    ScenarioCase(
        const ScenarioCase&) = delete;
    ScenarioCase& operator = (
        const ScenarioCase&) = delete;

    /* -- scenario */
    virtual ScenarioPtr filterScenario(
        ObjectPath& path_,
        TagsStack& tags_,
        ScenarioContainerPtr parent_,
        const RunnerFilter& name_filter_,
        const TagFilter& tag_filter_) const override;
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
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_SCENARIOCASE_H_ */

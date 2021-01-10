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

#ifndef OTest2_INCLUDE_OTEST2_SCENARIOSUITE_H_
#define OTest2_INCLUDE_OTEST2_SCENARIOSUITE_H_

#include <string>

#include <otest2/objectrepeaterfactoryptr.h>
#include <otest2/scenariocontainer.h>

namespace OTest2 {

class Tags;

/**
 * @brief A scenario object representing one test suite
 */
class ScenarioSuite : public ScenarioContainer {
  public:
    struct Impl;

  private:
    Impl* pimpl;

  public:
    /**
     * @brief Create scenario object of a test suite
     *
     * @param name_ Name of the suite
     * @param tags_ Tags assigned to the object
     * @param repeater_factory_ Factory of the repeater objects of the suite
     */
    explicit ScenarioSuite(
        const std::string& name_,
        const Tags& tags_,
        ObjectRepeaterFactoryPtr repeater_factory_);

    /**
     * @brief Dtor
     */
    virtual ~ScenarioSuite();

    /* -- avoid copying */
    ScenarioSuite(
        const ScenarioSuite&) = delete;
    ScenarioSuite& operator = (
        const ScenarioSuite&) = delete;

    /* -- scenario */
    virtual ScenarioPtr filterScenario(
        TagsStack& tags_,
        ScenarioContainerPtr parent_,
        const RunnerFilter& filter_) const override;
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

    /* -- scenario container */
    virtual void appendScenario(
        const std::string& name_,
        ScenarioPtr scenario_) override;
    virtual bool isEmpty() const noexcept;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_SCENARIOSUITE_H_ */

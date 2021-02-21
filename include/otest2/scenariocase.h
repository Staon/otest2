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
class ScenarioCaseBuilder;

/**
 * @brief Scenario object of a test case
 */
class ScenarioCase : public Scenario {
  private:
    struct Impl;
    Impl* pimpl;
    friend class ScenarioCaseBuilder;

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
     * @brief Ctor - instance with active test section
     *
     * @param name_ Name of the test case
     * @param section_path_ Path of the active section (relative to the case)
     * @param tags_ Tags assigned to the object
     * @param repeater_factory A factory of repeater objects of the test case
     */
    explicit ScenarioCase(
        const std::string& name_,
        const std::string& section_path_,
        const Tags& tags_,
        ObjectRepeaterFactoryPtr repeater_factory_);

    /**
     * @brief Create builder of the scenario case
     * @sa ScenarioCase::ScenarioCase()
     */
    static ScenarioCaseBuilder createBuilder(
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
        TagsStack& tags_,
        ScenarioContainerPtr parent_,
        const RunnerFilter& filter_) const override;
    virtual std::pair<std::string, ObjectRepeaterPtr> createRepeater(
        const Context& context_) const override;
    virtual void enterObject(
        const Context& context_) const noexcept override;
    virtual void leaveObject(
        const Context& context_) const noexcept override;
    virtual ScenarioIterPtr getChildren() const override;
};

/**
 * @brief A helper class for building of a case scenario (mainly composition
 *     of test sections)
 */
class ScenarioCaseBuilder {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    ScenarioCaseBuilder(
        const ScenarioCaseBuilder&) = delete;
    ScenarioCaseBuilder& operator = (
        const ScenarioCaseBuilder&) = delete;

    /**
     * @brief Ctor
     * @sa ScenarioCase::ScenarioCase()
     */
    explicit ScenarioCaseBuilder(
        const std::string& name_,
        const Tags& tags_,
        ObjectRepeaterFactoryPtr repeater_factory_);

    /**
     * @brief Move ctor
     */
    ScenarioCaseBuilder(
        ScenarioCaseBuilder&& other_);

    /**
     * @brief Dtor
     */
    ~ScenarioCaseBuilder();

    /**
     * @brief Push a child section
     *
     * @param name_ Name of the section
     */
    ScenarioCaseBuilder& pushSection(
        const std::string& name_);

    /**
     * @brief Pop section at current top
     */
    ScenarioCaseBuilder& popSection();

    /**
     * @brief Get the build scenario
     */
    ScenarioPtr getScenario();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_SCENARIOCASE_H_ */

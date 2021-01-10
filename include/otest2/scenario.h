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

#ifndef OTest2_INCLUDE_OTEST2_SCENARIO_H_
#define OTest2_INCLUDE_OTEST2_SCENARIO_H_

#include <utility>

#include <otest2/objectrepeaterptr.h>
#include <otest2/scenariocontainerptr.h>
#include <otest2/scenarioiterptr.h>
#include <otest2/scenarioptr.h>

namespace OTest2 {

class Context;
class ObjectPath;
class RunnerFilter;
class RunnerFilterTags;
class TagsStack;

/**
 * @brief Generic scenario object
 *
 * The scenario is responsible for scheduling of test runs. Generally,
 * it works as a kind of registry of suites and test cases. The scenario
 * can be filtered by tags or object names. The test run then follows
 * data in the scenario object.
 */
class Scenario {
  public:
    /**
     * @brief Ctor
     */
    Scenario();

    /**
     * @brief Dtor
     */
    virtual ~Scenario();

    /* -- avoid copying */
    Scenario(
        const Scenario&) = delete;
    Scenario& operator = (
        const Scenario&) = delete;

    /**
     * @brief Filter the scenario
     *
     * This method filters the scenario object. Objects which don't match
     * the filters are removed.
     *
     * @param tags_ Stack of object names and tags
     * @param parent_ Parent container
     * @param filter_ Used filter
     * @return The @a parent_ object if it's not null. Otherwise the new
     *     container object.
     */
    virtual ScenarioPtr filterScenario(
        TagsStack& tags_,
        ScenarioContainerPtr parent_,
        const RunnerFilter& filter_) const = 0;

    /**
     * @brief Create repeater object for testing object represented by this
     *     scenario object
     *
     * @param[in] context_ The OTest2 context
     * @param[out] object_name_ Name of the testing object
     * @return The name of the testing object and its repeater
     */
    virtual std::pair<std::string, ObjectRepeaterPtr> createRepeater(
        const Context& context_) const = 0;

    /**
     * @brief Report entering of the testing object
     *
     * As constructor methods may throw an exception causing failure of the test,
     * we report the entering into the object before actual creation of the
     * object.
     *
     * @param context_ The OTest2 context
     * @param decorated_name_ Decorated name of the testing object
     */
    virtual void reportEntering(
        const Context& context_,
        const std::string& decorated_name_) const noexcept = 0;

    /**
     * @brief Report leaving of the testing object
     *
     * @param context_ The OTest2 context
     * @param decorated_name_ Decorated name of the testign object
     * @param result_ Result of the testing object
     */
    virtual void reportLeaving(
        const Context& context_,
        const std::string& decorated_name_,
        bool result_) const noexcept = 0;

    /**
     * @brief Get iterator of children object
     */
    virtual ScenarioIterPtr getChildren() const = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_SCENARIO_H_ */

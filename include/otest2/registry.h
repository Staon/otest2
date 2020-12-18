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

#ifndef OTest2__INCLUDE_REGISTRY_H_
#define OTest2__INCLUDE_REGISTRY_H_

#include <string>

#include <otest2/scenarioiterptr.h>
#include <otest2/scenarioptr.h>

namespace OTest2 {

class RunnerFilter;
class TagFilter;

/**
 * @brief Test registry
 *
 * This class collects test suites.
 */
class Registry {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    Registry(
        const Registry&) = delete;
    Registry& operator =(
        const Registry&) = delete;

  public:
    /**
     * @brief Ctor
     */
    Registry();

    /**
     * @brief Dtor
     */
    ~Registry();

    /**
     * @brief Register a scenario object
     *
     * @param name_ Name of the scenario (name of the testing object)
     * @param scenario_ The scenario object
     */
    void registerScenario(
        const std::string& name_,
        ScenarioPtr scenario_);

    /**
     * @brief Set test name
     *
     * The test name is a name of the entire test as it's reported
     * into the reporter.
     *
     * @param name_ The test name
     */
    void setTestName(
        const std::string& name_);

    /**
     * @brief Get iterator of test roots
     *
     * @param name_filter_ Filter by object names
     * @param tag_filter_ Filter by object's tags
     */
    ScenarioIterPtr getTests(
        const RunnerFilter& name_filter_,
        const TagFilter& tag_filter_) const;

    /**
     * @brief Access of the global instances
     *
     * The global instances are used for automatic registering of suites.
     *
     * @param domain_ A test domain name. There can be several test domains.
     *     Each has own registry and can be run separately. The name is
     *     an unique identifier.
     */
    static Registry& instance(
        const std::string& domain_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_REGISTRY_H_ */

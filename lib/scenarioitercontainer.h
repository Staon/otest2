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

#ifndef OTest2_LIB_SCENARIOITERCONTAINER_H_
#define OTest2_LIB_SCENARIOITERCONTAINER_H_

#include <map>
#include <vector>

#include <scenarioiter.h>
#include <scenarioptr.h>

namespace OTest2 {

class ScenarioIterContainer : public ScenarioIter {
  private:
    typedef std::vector<ScenarioPtr> Items;
    Items items;
    int index;

  public:
    explicit ScenarioIterContainer(
        const std::map<std::string, ScenarioPtr>& children_);
    explicit ScenarioIterContainer(
        const std::vector<ScenarioPtr>& children_);
    virtual ~ScenarioIterContainer() = default;

    /* -- avoid copying */
    ScenarioIterContainer(
        const ScenarioIterContainer&) = delete;
    ScenarioIterContainer& operator = (
        const ScenarioIterContainer&) = delete;

    /* -- scenario iterator */
    virtual bool isValid() const noexcept override;
    virtual ScenarioPtr getScenario() const noexcept override;
    virtual void next() noexcept override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_LIB_SCENARIOITERCONTAINER_H_ */

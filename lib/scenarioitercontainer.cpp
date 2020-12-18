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
#include "scenarioitercontainer.h"

#include <assert.h>

namespace OTest2 {

ScenarioIterContainer::ScenarioIterContainer(
    const std::map<std::string, ScenarioPtr>& children_) :
  items(),
  index(0) {
  items.reserve(children_.size());
  for(const auto& item_ : children_) {
    items.push_back(item_.second);
  }
}

ScenarioIterContainer::ScenarioIterContainer(
    const std::vector<ScenarioPtr>& children_) :
  items(children_),
  index(0) {

}

bool ScenarioIterContainer::isValid() const noexcept {
  return index < items.size();
}

ScenarioPtr ScenarioIterContainer::getScenario() const noexcept {
  assert(index < items.size());
  return items[index];
}

void ScenarioIterContainer::next() noexcept {
  ++index;
}

} /* -- namespace OTest2 */

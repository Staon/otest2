/*
 * Copyright (C) 2021 Ondrej Starek
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
#include <runnerfilteruntagged.h>

#include <vector>

#include <tags.h>
#include <tagsstack.h>

namespace OTest2 {

RunnerFilterUntagged::RunnerFilterUntagged() = default;

RunnerFilterUntagged::~RunnerFilterUntagged() = default;

bool RunnerFilterUntagged::filterPath(
    const TagsStack& path_) const noexcept {
  std::vector<TagsStack::TagRecord> tags_;
  path_.fillTags(tags_);
  for(const auto& item_ : tags_) {
    if(!item_.tags.isEmpty())
      return true;
  }
  return false;
}

} /* -- namespace OTest2 */

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

#include <context.h>

namespace OTest2 {

Context::Context(
    CommandStack* command_stack_,
    SemanticStack* semantic_stack_,
    ObjectPath* object_path_,
    TimeSource* time_source_,
    ExcCatcher* exception_catcher_,
    Reporter* reporter_,
    RunnerFilter* runner_filter_,
    TestMarkFactory* test_mark_factory_,
    TestMarkStorage* test_mark_storage_,
    UserData* user_data_,
    TagsStack* tags_stack_,
    TagFilter* tag_filter_) :
  command_stack(command_stack_),
  semantic_stack(semantic_stack_),
  object_path(object_path_),
  time_source(time_source_),
  exception_catcher(exception_catcher_),
  reporter(reporter_),
  runner_filter(runner_filter_),
  test_mark_factory(test_mark_factory_),
  test_mark_storage(test_mark_storage_),
  user_data(user_data_),
  tags_stack(tags_stack_),
  tag_filter(tag_filter_) {

}

Context::~Context() {

}

} /* -- namespace OTest2 */

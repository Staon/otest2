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

#ifndef OTest2INCLUDE_CONTEXT_H_
#define OTest2INCLUDE_CONTEXT_H_

namespace OTest2 {

class CommandStack;
class ExcCatcher;
class ObjectPath;
class Reporter;
class RunnerFilter;
class SemanticStack;
class TagFilter;
class TagsStack;
class TestMarkFactory;
class TestMarkStorage;
class TimeSource;
class UserData;

/**
 * @brief OTest2 runtime context
 */
class Context {
  public:
    CommandStack* const command_stack;
    SemanticStack* const semantic_stack;
    ObjectPath* const object_path;
    TimeSource* const time_source;
    ExcCatcher* const exception_catcher;
    Reporter* const reporter;
    RunnerFilter* const runner_filter;
    TestMarkFactory* const test_mark_factory;
    TestMarkStorage* const test_mark_storage;
    UserData* const user_data;
    TagsStack* const tags_stack;
    TagFilter* const tag_filter;

    /* -- avoid copying */
    Context(
        const Context&) = delete;
    Context& operator =(
        const Context&) = delete;

    /**
     * @brief Ctor
     */
    explicit Context(
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
        TagFilter* tag_filter_);

    /**
     * @brief Dtor
     */
    ~Context();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_CONTEXT_H_ */

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

#ifndef OTest2__INCLUDE_OTEST2_RUNNERORDINARY_H_
#define OTest2__INCLUDE_OTEST2_RUNNERORDINARY_H_

#include <string>

#include <otest2/runner.h>

namespace OTest2 {

class ExcCatcher;
class Registry;
class Reporter;
class RunnerFilter;
class TestMarkFactory;
class TestMarkStorage;
class TimeSource;
class UserData;

/**
 * @brief Ordinary implementation of the Runner interface
 *
 * This implementation keeps a stack of commands and runs them until the stack
 * is empty.
 */
class RunnerOrdinary : public Runner {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    RunnerOrdinary(
        const RunnerOrdinary&) = delete;
    RunnerOrdinary& operator =(
        const RunnerOrdinary&) = delete;

    /**
     * @brief Ctor
     *
     * @param time_source_ A source of current time. The ownership is not taken.
     * @param exc_catcher_ An exception catcher. The ownership is not taken.
     * @param reporter_ A reporter object. The ownership is not taken.
     * @param registry_ A registry of suites to be run. The ownership is not taken.
     * @param runner_filter_ A runner filter - an object which allows specification
     *     of tests which should be run.
     * @param test_mark_factory_ A factory of test mark nodes. The ownership
     *     is not taken.
     * @param test_mark_storage_ Storage of test marks. The ownership is not
     *     taken.
     * @param user_data_ A container keeping user's data passed into the test.
     *     The ownership is not taken.
     * @param name_ Name of the test (shown in the test report)
     */
    explicit RunnerOrdinary(
        TimeSource* time_source_,
        ExcCatcher* exc_catcher_,
        Reporter* reporter_,
        Registry* registry_,
        RunnerFilter* runner_filter_,
        TestMarkFactory* test_mark_factory_,
        TestMarkStorage* test_mark_storage_,
        UserData* user_data_,
        const std::string& name_);

    /**
     * @brief Dtor
     */
    virtual ~RunnerOrdinary();

    /* -- runner interface */
    virtual RunnerResult runNext() override;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_RUNNERORDINARY_H_ */

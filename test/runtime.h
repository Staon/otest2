/*
 * Copyright (C) 2019 Ondrej Starek
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

#ifndef OTest2__TEST_RUNTIME_H_
#define OTest2__TEST_RUNTIME_H_

#include <memory>
#include <otest2/exccatcherordinary.h>
#include <otest2/runnerfilterone.h>
#include <otest2/runnerordinary.h>
#include <otest2/tagfilter.h>
#include <otest2/testmarkfactory.h>
#include <otest2/testmarkstorage.h>
#include <otest2/userdata.h>

#include "reportermock.h"
#include "timesourcemock.h"

namespace OTest2 {

namespace Test {


/**
 * @brief Generic runtime of the OTest2 self tests
 */
class Runtime {
  public:
    TimeSourceMock time_source;
    ExcCatcherOrdinary exc_catcher;
    ReporterMock reporter;
    RunnerFilterOne runner_filter;
    TestMarkFactory test_mark_factory;
    std::unique_ptr<TestMarkStorage> test_mark_storage;
    UserData user_data;
    TagFilter tag_filter;
    RunnerOrdinary runner;

    static struct ReportPaths{} report_paths_mark;
    static struct Tags{} tags_mark;

  public:
    /* -- avoid copying */
    Runtime(
        const Runtime&) = delete;
    Runtime& operator =(
        const Runtime&) = delete;

    /**
     * @brief Ctor
     *
     * @param suite_name_ Name of the suite to be run
     * @param case_name_ Name of the case to be run. If it's empty the entire
     *     suite is run.
     */
    explicit Runtime(
        const std::string& suite_name_,
        const std::string& case_name_);

    /**
     * @brief Ctor
     *
     * @param suite_name_ Name of the suite to be run
     * @param case_name_ Name of the case to be run. If it's empty the entire
     *     suite is run.
     * @param report_paths_ A mark that the object paths shall be reported.
     */
    explicit Runtime(
        const std::string& suite_name_,
        const std::string& case_name_,
        const ReportPaths& report_paths_);

    /**
     * @brief Ctor
     *
     * @param suite_name_ Name of the suite to be run
     * @param case_name_ Name of the case to be run. If it's empty the entire
     *     suite is run.
     * @param regression_file_ Name of the regression (test marks) file
     */
    explicit Runtime(
        const std::string& suite_name_,
        const std::string& case_name_,
        const std::string& regression_file_);

    /**
     * @brief Ctor
     *
     * @param suite_name_ Name of the suite to be run
     * @param case_name_ Name of the case to be run. If it's empty the entire
     *     suite is run.
     * @param regression_file_ Name of the regression (test marks) file
     * @param tag_mark_ A mark that the tag expression is set
     * @param tag_expression_ The tag expression
     */
    explicit Runtime(
        const std::string& suite_name_,
        const std::string& case_name_,
        const Tags&,
        const std::string& tag_expression_);

    /**
     * @brief Ctor
     *
     * @param suite_name_ Name of the suite to be run
     * @param case_name_ Name of the case to be run. If it's empty the entire
     *     suite is run.
     * @param regression_file_ Name of the regression (test marks) file
     * @param tag_expression_ Specified tag expression
     */
    explicit Runtime(
        const std::string& suite_name_,
        const std::string& case_name_,
        const std::string& regression_file_,
        const std::string& tag_expression_);

  private:
    static struct InternalCtor{} internal_ctor;
    explicit Runtime(
        const InternalCtor&,
        const std::string& suite_name_,
        const std::string& case_name_,
        bool report_paths_,
        const std::string& regression_file_,
        const std::string& tag_expression_);

  public:
    /**
     * @brief Dtor
     */
    ~Runtime();

    /**
     * @brief Run the pre-configured suite or test case
     *
     * @return True if the test ended successfully.
     */
    bool runTheTest();
};

} /* namespace Test */

} /* namespace OTest2 */

#endif /* OTest2__TEST_RUNTIME_H_ */

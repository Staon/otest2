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

#include <otest2/runnerfilterone.h>
#include <otest2/runnerordinary.h>

#include "reportermock.h"

namespace OTest2 {

namespace Test {

/**
 * @brief Generic runtime of the OTest2 self tests
 */
class Runtime {
  public:
    ReporterMock reporter;
    RunnerFilterOne runner_filter;
    RunnerOrdinary runner;

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

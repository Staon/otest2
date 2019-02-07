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

#ifndef OTest2__INCLUDE_OTEST2_RUNNERFILTERONE_H_
#define OTest2__INCLUDE_OTEST2_RUNNERFILTERONE_H_

#include <string>

#include <otest2/runnerfilter.h>

namespace OTest2 {

/**
 * @brief A filter which allows run of only one test case or only one suite
 */
class RunnerFilterOne : public RunnerFilter {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    RunnerFilterOne(
        const RunnerFilterOne&) = delete;
    RunnerFilterOne& operator =(
        const RunnerFilterOne&) = delete;

    /**
     * @brief Ctor - allow run of one entire suite
     *
     * @param suite_ Name of the suite
     */
    explicit RunnerFilterOne(
        const std::string& suite_);

    /**
     * @brief Ctor - allow run of only one test case
     *
     * @param suite_ Name of the suite
     * @param testcase_ Name of the test case. If it's empty the entire suite
     *     is run.
     */
    explicit RunnerFilterOne(
        const std::string& suite_,
        const std::string& testcase_);

    /**
     * @brief Dtor
     */
    virtual ~RunnerFilterOne();

    /* -- runner filter interface */
    virtual bool filterSuite(
        const std::string& suite_name_) const;
    virtual bool filterCase(
        const std::string& suite_name_,
        const std::string& case_name_) const;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_RUNNERFILTERONE_H_ */

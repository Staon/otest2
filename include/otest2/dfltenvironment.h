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

#ifndef OTest2__INCLUDE_OTEST2_DFLTENVIRONMENT_H_
#define OTest2__INCLUDE_OTEST2_DFLTENVIRONMENT_H_

#include <string>

namespace OTest2 {

class ExcCatcher;
class Reporter;
class Runner;

/**
 * @brief Default test environment
 */
class DfltEnvironment {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor - default running environment
     *
     * @param testname_ Name of the test as it's shown in the reports
     */
    explicit DfltEnvironment(
        const std::string& testname_);

    /**
     * @brief Ctor
     *
     * @param argc_ Number of command line arguments
     * @param argv_ The command line arguments
     */
    DfltEnvironment(
        int argc_,
        char* argv_[]);

    /**
     * @brief Dtor
     */
    ~DfltEnvironment();

    /**
     * @brief Append a test reporter
     *
     * @param reporter_ The reporter. The ownership is not taken, the object
     *     must exist for whole lifetime of this object.
     */
    void addReporter(
        Reporter* reporter_);

    /**
     * @brief Set exception catcher
     *
     * @param catcher_ The catcher. The ownership is not taken, the object
     *     must exist for the whole lifetime of this object.
     */
    void setExceptionCatcher(
        ExcCatcher* catcher_);

    /**
     * @brief Get constructed runner
     */
    Runner& getRunner();
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_DFLTENVIRONMENT_H_ */

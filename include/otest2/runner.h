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

#ifndef OTest2__INCLUDE_OTEST2_RUNNER_H_
#define OTest2__INCLUDE_OTEST2_RUNNER_H_

namespace OTest2 {

/**
 * @brief Result of test run
 *
 * This structure is returned by the runner after one test step. There
 * are several possible results:
 *   - the test hasn't finished yet, wait for some time and run the runner
 *     again,
 *   - the test has successfully finished,
 *   - the test has failed.
 *
 * The user must check the isFinished() value. If the test is finished,
 * he can get the test result by calling the getResult() method. If the test
 * isn't finished, he must get the delay value getDelayMS(), wait for
 * specified time (it can be zero!) and run the test runner again.
 */
class RunnerResult {
  private:
    bool running;
    bool result;
    int delay_ms;

  public:
    /**
     * @brief Default ctor - finished failed result
     */
    RunnerResult();

    /**
     * @brief Ctor
     *
     * @param running_ True if the test hasn't finished yet.
     * @param result_ Result of the test. It's valid only if the @a running_
     *     is false.
     * @param delay_ms_ Delay time in milliseconds. It's valid only if the
     *     @a running_ is true.
     */
    RunnerResult(
        bool running_,
        bool result_,
        int delay_ms_);

    /**
     * @brief Copy ctor
     */
    RunnerResult(
        const RunnerResult& src_);

    /**
     * @brief Dtor
     */
    ~RunnerResult();

    /**
     * @brief Swap contents
     */
    void swap(
        RunnerResult& r2_) noexcept;

    /**
     * @brief Copy operator
     */
    RunnerResult& operator = (
        const RunnerResult& src_);

    /**
     * @brief Check whether the test has already finished
     */
    bool isFinished() const;

    /**
     * @brief Get delay between two steps in milliseconds
     *
     * @note This value is valid only if isFinished() is false!
     */
    int getDelayMS() const;

    /**
     * @brief Get result of the test - true if the test has passed
     *
     * @note This value is valid only if isFinished() is true!
     */
    bool getResult() const;
};

/**
 * @brief Generic test runner
 */
class Runner {
  public:
    /* -- avoid copying */
    Runner(
        const Runner&) = delete;
    Runner& operator =(
        const Runner&) = delete;

    /**
     * @brief Ctor
     */
    Runner();

    /**
     * @brief Dtor
     */
    virtual ~Runner();

    /**
     * @brief Run next pack of work
     *
     * @return A runner result. @sa the RunnerResult class for description
     *     how the test result is returned.
     */
    virtual RunnerResult runNext() = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_RUNNER_H_ */

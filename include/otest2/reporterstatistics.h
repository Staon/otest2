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

#ifndef OTest2__INCLUDE_OTEST2_REPORTERSTATISTICS_H_
#define OTest2__INCLUDE_OTEST2_REPORTERSTATISTICS_H_

namespace OTest2 {

/**
 * @brief A helper class keeping statistic counters
 *
 * This class keeps counters about running tests.
 */
class ReporterStatistics {
  private:
    int suites_ok;
    int suites_failed;
    int cases_ok;
    int cases_failed;
    int assertions_ok;
    int assertions_failed;

  public:
    /* -- avoid copying */
    ReporterStatistics(
        const ReporterStatistics&) = delete;
    ReporterStatistics& operator =(
        const ReporterStatistics&) = delete;

    /**
     * @brief Ctor
     */
    ReporterStatistics();

    /**
     * @brief Dtor
     */
    ~ReporterStatistics();

    /**
     * @brief Count new suite
     *
     * @param result_ Result of the suite
     */
    void reportSuite(
        bool result_);

    /**
     * @brief Count new test case
     *
     * @param result_ Result of the case
     */
    void reportCase(
        bool result_);

    /**
     * @brief Count new assertion
     *
     * @param result_ Result of the assertion
     */
    void reportAssertion(
        bool result_);

    /**
     * @brief Get count of suites
     */
    int getSuites() const;

    /**
     * @brief Get count of passed suites
     */
    int getSuitesOK() const;

    /**
     * @brief Get count of failed suites
     */
    int getSuitesFailed() const;

    /**
     * @brief Get count of test cases
     */
    int getCases() const;

    /**
     * @brief Get count of passed test cases
     */
    int getCasesOK() const;

    /**
     * @brief Get count of failed test cases
     */
    int getCasesFailed() const;

    /**
     * @brief Get count of assertions
     */
    int getAssertions() const;

    /**
     * @brief Get count of passed assertions
     */
    int getAssertionsOK() const;

    /**
     * @brief Get count of failed assertions
     */
    int getAssertionsFailed() const;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_REPORTERSTATISTICS_H_ */

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

#include <reporterstatistics.h>

namespace OTest2 {

ReporterStatistics::ReporterStatistics() :
  suites_ok(0),
  suites_failed(0),
  cases_ok(0),
  cases_failed(0),
  assertions_ok(0),
  assertions_failed(0) {

}

ReporterStatistics::~ReporterStatistics() {

}

void ReporterStatistics::reportSuite(
    bool result_) {
  if(result_)
    ++suites_ok;
  else
    ++suites_failed;
}

void ReporterStatistics::reportCase(
    bool result_) {
  if(result_)
    ++cases_ok;
  else
    ++cases_failed;
}

void ReporterStatistics::reportAssertion(
    bool result_) {
  if(result_)
    ++assertions_ok;
  else
    ++assertions_failed;
}

int ReporterStatistics::getSuites() const {
  return suites_ok + suites_failed;
}

int ReporterStatistics::getSuitesOK() const {
  return suites_ok;
}

int ReporterStatistics::getSuitesFailed() const {
  return suites_failed;
}

int ReporterStatistics::getCases() const {
  return cases_ok + cases_failed;
}

int ReporterStatistics::getCasesOK() const {
  return cases_ok;
}

int ReporterStatistics::getCasesFailed() const {
  return cases_failed;
}

int ReporterStatistics::getAssertions() const {
  return assertions_ok + assertions_failed;
}

int ReporterStatistics::getAssertionsOK() const {
  return assertions_ok;
}

int ReporterStatistics::getAssertionsFailed() const {
  return assertions_failed;
}

} /* namespace OTest2 */

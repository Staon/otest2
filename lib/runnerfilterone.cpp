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

#include <runnerfilterone.h>

#include <assert.h>
#include <string>

#include <utils.h>

namespace OTest2 {

struct RunnerFilterOne::Impl {
  public:
    std::string suite_name;
    std::string case_name;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        const std::string& suite_name_,
        const std::string& case_name_);
    ~Impl();
};

RunnerFilterOne::Impl::Impl(
    const std::string& suite_name_,
    const std::string& case_name_) :
  suite_name(suite_name_),
  case_name(case_name_) {

}

RunnerFilterOne::Impl::~Impl() {

}

RunnerFilterOne::RunnerFilterOne(
    const std::string& suite_) :
  pimpl(new Impl(suite_, "")) {
  assert(!pimpl->suite_name.empty());

}

RunnerFilterOne::RunnerFilterOne(
    const std::string& suite_,
    const std::string& testcase_) :
  pimpl(new Impl(suite_, testcase_)) {
  assert(!pimpl->suite_name.empty());

}

RunnerFilterOne::~RunnerFilterOne() {
  odelete(pimpl);
}

bool RunnerFilterOne::filterSuite(
    const std::string& suite_name_) const {
  return suite_name_ != pimpl->suite_name;
}

bool RunnerFilterOne::filterCase(
    const std::string& suite_name_,
    const std::string& case_name_) const {
  if(suite_name_ != pimpl->suite_name)
    return true;  /* -- filter test cases of all other suites */
  if(pimpl->case_name.empty())
    return false; /* -- the entire suite should be run */
  return case_name_ != pimpl->case_name;
}

} /* namespace OTest2 */
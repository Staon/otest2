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

#include <objectpath.h>
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

std::unique_ptr<RunnerFilterOne> RunnerFilterOne::fromPath(
    const std::string& object_path_) {
  std::string suite_;
  std::string testcase_;

  auto index_(object_path_.find("::"));
  if(index_ == std::string::npos) {
    /* -- one whole suite */
    return ::OTest2::make_unique<RunnerFilterOne>(object_path_);
  }
  else {
    /* -- just one testcase */
    std::string suite_(object_path_.substr(0, index_));
    std::string testcase_(object_path_.substr(index_ + 2));
    return ::OTest2::make_unique<RunnerFilterOne>(suite_, testcase_);
  }
}

bool RunnerFilterOne::filterPath(
    const ObjectPath& path_) const noexcept {
  /* -- filter suite name */
  if(!path_.doesMatchSuite(pimpl->suite_name))
    return true;

  /* -- filter case name */
  if(!pimpl->case_name.empty() && !path_.doesMatchCase(pimpl->case_name))
    return true;

  return false;
}

} /* namespace OTest2 */

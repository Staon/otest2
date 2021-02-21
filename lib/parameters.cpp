/*
 * Copyright (C) 2021 Ondrej Starek
 *
 * This file is part of OTest2
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

#include <parameters.h>

#include <assert.h>
#include <map>
#include <sstream>

#include <utils.h>

namespace OTest2 {

struct Parameters::Impl {
    typedef std::multimap<std::string, std::string> Params;
    Params params;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    Impl() = default;
    ~Impl() = default;
};

Parameters::Parameters() :
  pimpl(new Impl) {

}

Parameters::Parameters(
    Parameters&& other_) noexcept :
  pimpl(other_.pimpl) {
  other_.pimpl = nullptr;
}

Parameters::~Parameters() {
  odelete(pimpl);
}

void Parameters::swap(
    Parameters& other_) noexcept {
  std::swap(pimpl, other_.pimpl);
}

Parameters& Parameters::operator = (
    Parameters&& other_) noexcept {
  if(this != &other_) {
    swap(other_);
    odelete(other_.pimpl);
  }
  return *this;
}

void Parameters::appendParameter(
    const std::string& name_,
    const std::string& value_) {
  assert(!name_.empty());
  pimpl->params.insert(Impl::Params::value_type(name_, value_));
}

std::string Parameters::mixWithName(
    const std::string& name_) const {
  std::ostringstream oss_;

  oss_ << name_;

  if(!pimpl->params.empty()) {
    oss_ << " (";

    bool first_(true);
    for(const auto& param : pimpl->params) {
      if(first_)
        first_ = false;
      else
        oss_ << ", ";
      oss_ << param.first << ": " << param.second;
    }

    oss_ << ")";
  }

  return oss_.str();
}

} /* -- namespace OTest2 */

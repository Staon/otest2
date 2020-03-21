/*
 * Copyright (C) 2020 Ondrej Starek
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

#include <otest2/objectpath.h>

#include <algorithm>
#include <assert.h>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <infixiterator.h>
#include <utils.h>

namespace OTest2 {

struct ObjectPath::Impl {
    typedef std::vector<std::string> PathStack;
    PathStack path_stack_;
};

ObjectPath::ObjectPath() :
  pimpl(new Impl) {

}

ObjectPath::~ObjectPath() {
  odelete(pimpl);
}

void ObjectPath::pushName(
    const std::string& name_) {
  assert(!name_.empty());
  pimpl->path_stack_.push_back(name_);
}

void ObjectPath::popName() {
  assert(!pimpl->path_stack_.empty());
  pimpl->path_stack_.pop_back();
}

std::string ObjectPath::getCurrentPath() const {
  std::ostringstream oss_;
  std::copy(
      pimpl->path_stack_.begin(),
      pimpl->path_stack_.end(),
      InfixIterator<std::string>(&oss_, "::"));
  return oss_.str();
}

std::string ObjectPath::getRegressionKey(
    const std::string& local_key_) const {
  std::ostringstream oss_;
  std::copy(
      pimpl->path_stack_.begin(),
      pimpl->path_stack_.end(),
      std::ostream_iterator<std::string>(oss_, ">>"));
  oss_ << local_key_;
  return oss_.str();
}

} /* -- namespace OTest2 */

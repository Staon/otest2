/*
 * Copyright (C) 2020 Ondrej Starek
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
#include <objectrepeateronce.h>

#include <assert.h>
#include <string>

namespace OTest2 {

ObjectRepeaterOnce::ObjectRepeaterOnce() :
  created(false) {

}

ObjectRepeaterOnce::~ObjectRepeaterOnce() {

}

bool ObjectRepeaterOnce::isNextRun(
    const Context& context_) const {
  return !created;
}

std::string ObjectRepeaterOnce::transformName(
    const Context& context_,
    const std::string& name_) const {
  assert(!created);
  return name_;
}

ObjectScenarioPtr ObjectRepeaterOnce::createObject(
    const Context& context_,
    const std::string& decorated_name_,
    ObjectPtr parent_) {
  assert(!created);
  created = true;
  return doCreateObject(context_, decorated_name_, parent_);
}

} /* -- namespace OTest2 */

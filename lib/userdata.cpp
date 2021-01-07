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
#include <userdata.h>

#include <unordered_map>
#include <memory>
#include <utility>

#include <utils.h>

namespace OTest2 {

UserDataMissingException::UserDataMissingException(
    const std::string& name_) :
  name(name_) {

}

UserDataMissingException::UserDataMissingException(
    UserDataMissingException&& exc_) noexcept :
  Exception(std::move(exc_)),
  name(std::move(exc_.name)) {

}

UserDataMissingException::~UserDataMissingException() {

}

std::string UserDataMissingException::reason() const {
  return "missing user datum '" + name + "'";
}

UserDataWrongTypeException::UserDataWrongTypeException(
    const std::string& name_,
    const std::string& actual_,
    const std::string& expected_) :
  name(name_),
  actual_type(actual_),
  expected_type(expected_) {

}

UserDataWrongTypeException::UserDataWrongTypeException(
    UserDataWrongTypeException&& exc_) noexcept :
  Exception(std::move(exc_)),
  name(std::move(exc_.name)),
  actual_type(std::move(exc_.actual_type)),
  expected_type(std::move(exc_.expected_type)) {

}

UserDataWrongTypeException::~UserDataWrongTypeException() {

}

std::string UserDataWrongTypeException::reason() const {
  return "requested type of the user datum '" + name + "' is '"
      + expected_type + "' but the type '" + actual_type + "' is set";
}

UserData::Datum::Datum() {

}

UserData::Datum::~Datum() {

}

struct UserData::Impl {
    typedef std::unordered_map<std::string, std::unique_ptr<UserData::Datum>> Data;
    Data data;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    Impl();
    ~Impl();
};

UserData::Impl::Impl() {

}

UserData::Impl::~Impl() {

}

UserData::UserData() :
  pimpl(new Impl) {

}

UserData::~UserData() {
  odelete(pimpl);
}

void UserData::doSetDatum(
    const std::string& name_,
    std::unique_ptr<Datum>&& datum_) {
  pimpl->data.erase(name_);
  pimpl->data.emplace(name_, std::move(datum_));
}

UserData::Datum* UserData::doGetDatum(
    const std::string& name_) const {
  auto iter_(pimpl->data.find(name_));
  if(iter_ == pimpl->data.end())
    throw UserDataMissingException(name_);
  return (*iter_).second.get();
}

} /* -- namespace OTest2 */

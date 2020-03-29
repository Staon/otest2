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

#include <testmarkfactory.h>

#include <assert.h>
#include <unordered_map>

#include <exctestmarkin.h>
#include <testmarkbool.h>
#include <testmarkfloat.h>
#include <testmarkint.h>
#include <testmarklist.h>
#include <testmarkmap.h>
#include <testmarknull.h>
#include <testmarkstring.h>
#include <utils.h>

namespace OTest2 {

struct TestMarkFactory::Impl {
    std::unordered_map<std::string, std::function<TestMarkPtr()>> ctors;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    Impl();
    ~Impl();
};

TestMarkFactory::Impl::Impl() {

}

TestMarkFactory::Impl::~Impl() {

}

void TestMarkFactory::doRegisterRecord(
    const std::string& typemark_,
    std::function<TestMarkPtr()> ctor_) {
  auto result_(pimpl->ctors.insert({typemark_, ctor_}));
  assert(result_.second);
}

TestMarkFactory::TestMarkFactory() :
  pimpl(new Impl()) {

  /* -- registration of standard testmark objects */
  registerMark<TestMarkNull>();
  registerMark<TestMarkBool>();
  registerMark<TestMarkInt>();
  registerMark<TestMarkFloat>();
  registerMark<TestMarkString>();
  registerMark<TestMarkList>();
  registerMark<TestMarkMap>();
}

TestMarkFactory::~TestMarkFactory() {
  odelete(pimpl);
}

TestMarkPtr TestMarkFactory::createMark(
    const std::string& typemark_) {
  auto iter_(pimpl->ctors.find(typemark_));
  if(iter_ == pimpl->ctors.end())
    throw ExcTestMarkIn("unknown type mark '" + typemark_ + "'");
  return (*iter_).second();
}

} /* -- namespace OTest2 */

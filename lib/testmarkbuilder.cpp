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
#include <testmarkbuilder.h>

#include <assert.h>
#include <vector>

#include <testmarkbool.h>
#include <testmarkfloat.h>
#include <testmarkint.h>
#include <testmarklist.h>
#include <testmarkmap.h>
#include <testmarknull.h>
#include <testmarkstring.h>
#include <utils.h>

namespace OTest2 {

struct TestMarkBuilder::Impl {
    TestMarkPtr root;
    class RootContainer : public Container {
      public:
        TestMarkPtr* root;
        explicit RootContainer(
            TestMarkPtr* root_);
        virtual void append(
            const std::string& key_,
            TestMarkPtr mark_);
        virtual TestMark* getMark();
        virtual TestMarkHashCode getHashCode() const;
    };
    struct Record {
        std::string key;
        std::unique_ptr<Container> container;

        explicit Record(
            std::string key_,
            std::unique_ptr<Container>&& container_);
    };
    typedef std::vector<Record> Stack;
    Stack stack;

    void appendItem(
        TestMarkPtr item_);
    void appendItem(
        TestMark* item_);
};

TestMarkBuilder::Impl::RootContainer::RootContainer(
    TestMarkPtr* root_) :
  root(root_) {
  assert(root != nullptr);
}

void TestMarkBuilder::Impl::RootContainer::append(
    const std::string& key_,
    TestMarkPtr mark_) {
  assert(key_.empty() && mark_ != nullptr && *root == nullptr);
  *root = mark_;
}

TestMark* TestMarkBuilder::Impl::RootContainer::getMark() {
  assert(false);
  return nullptr;
}

TestMarkHashCode TestMarkBuilder::Impl::RootContainer::getHashCode() const {
  return TestMarkHashCode();
}

TestMarkBuilder::Impl::Record::Record(
    std::string key_,
    std::unique_ptr<Container>&& container_) :
  key(std::move(key_)),
  container(std::move(container_)) {

}

void TestMarkBuilder::Impl::appendItem(
    TestMarkPtr item_) {
  assert(item_ != nullptr);

  Record& record_(stack.back());
  record_.container->append(record_.key, item_);
  record_.key = "";
}

void TestMarkBuilder::Impl::appendItem(
    TestMark* item_) {
  assert(item_ != nullptr);

  TestMarkPtr node_(item_);
  appendItem(node_);
}

TestMarkBuilder::TestMarkBuilder() :
  pimpl(new Impl) {
  pimpl->stack.emplace_back(
      "",
      ::OTest2::make_unique<Impl::RootContainer>(&pimpl->root));
}

TestMarkBuilder::~TestMarkBuilder() {
  odelete(pimpl);
}

void TestMarkBuilder::setKey(
    const std::string& key_) {
  assert(pimpl->stack.back().key.empty() && !key_.empty());
  pimpl->stack.back().key = key_;
}

void TestMarkBuilder::appendMark(
    TestMarkPtr mark_) {
  pimpl->appendItem(mark_);
}

void TestMarkBuilder::appendNull() {
  pimpl->appendItem(new TestMarkNull);
}

void TestMarkBuilder::appendBool(
    bool value_) {
  pimpl->appendItem(new TestMarkBool(value_));
}

void TestMarkBuilder::appendInt(
    int64_t value_) {
  pimpl->appendItem(new TestMarkInt(value_));
}

void TestMarkBuilder::appendFloat(
    long double value_) {
  pimpl->appendItem(new TestMarkFloat(value_));
}

void TestMarkBuilder::appendString(
    const std::string& value_) {
  pimpl->appendItem(new TestMarkString(value_));
}

void TestMarkBuilder::openContainerImpl(
    std::unique_ptr<typename TestMarkBuilder::Container>&& container_) {
  pimpl->stack.emplace_back("", std::move(container_));
}

void TestMarkBuilder::openList() {
  openContainer(::OTest2::make_unique<TestMarkList>());
}

void TestMarkBuilder::openList(
    const std::string& prefix_) {
  openContainer(::OTest2::make_unique<TestMarkList>(prefix_));
}

void TestMarkBuilder::openMap() {
  openContainer(::OTest2::make_unique<TestMarkMap>());
}

void TestMarkBuilder::openMap(
    const std::string& prefix_) {
  openContainer(::OTest2::make_unique<TestMarkMap>(prefix_));
}

void TestMarkBuilder::closeContainer() {
  std::unique_ptr<TestMark> container_(pimpl->stack.back().container->getMark());
  pimpl->stack.pop_back();
  pimpl->appendItem(container_.release());
}

TestMarkPtr TestMarkBuilder::stealMark() const {
  assert(pimpl->stack.size() == 1);
  assert(pimpl->root != nullptr);
  TestMarkPtr retval(pimpl->root);
  pimpl->root.reset();
  return retval;
}

} /* namespace OTest2 */

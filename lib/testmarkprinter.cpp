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

#include <testmarkprinter.h>

#include <assert.h>

#include <testmarkformatter.h>
#include <utils.h>

namespace OTest2 {

struct TestMarkPrinter::Impl {
    const std::vector<TestMark::LinearizedRecord>* array;
    struct StackRecord {
        const TestMark::LinearizedRecord* mark;
        void (TestMarkFormatter::* open)(const std::string&, const TestMark*, int);
        void (TestMarkFormatter::* close)(const std::string&, const TestMark*, int);
        int indent;
    };
    std::vector<StackRecord> stack;
    int& index;

    explicit Impl(
        const std::vector<TestMark::LinearizedRecord>* array_,
        int& index_);

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    void printTop(
        TestMarkFormatter& formatter_,
        bool open_);
    bool handleItem(
        TestMarkFormatter& formatter_,
        void (TestMarkFormatter::* open_)(const std::string&, const TestMark*, int),
        void (TestMarkFormatter::* close_)(const std::string&, const TestMark*, int));
};

TestMarkPrinter::Impl::Impl(
    const std::vector<TestMark::LinearizedRecord>* array_,
    int& index_) :
  array(array_),
  stack(),
  index(index_) {
  assert(array != nullptr);
  index = 0;
}

void TestMarkPrinter::Impl::printTop(
    TestMarkFormatter& formatter_,
    bool open_) {
  assert(!stack.empty());

  const auto& top_(stack.back());
  if(top_.open != nullptr) {
    if(open_)
      (formatter_.*top_.open)(top_.mark->label, top_.mark->me, top_.indent);
    else
      (formatter_.*top_.close)(top_.mark->label, top_.mark->me, top_.indent);
  }
}

bool TestMarkPrinter::Impl::handleItem(
    TestMarkFormatter& formatter_,
    void (TestMarkFormatter::* open_)(const std::string&, const TestMark*, int),
    void (TestMarkFormatter::* close_)(const std::string&, const TestMark*, int)) {
  assert(array != nullptr);

  /* -- out of the end */
  if(index >= array->size()) {
    /* -- finish all opened nodes */
    if(!stack.empty()) {
      printTop(formatter_, false);
      stack.pop_back();
      return true;
    }

    /* -- empty stack, empty input sequence => final end */
    return false;
  }

  auto& curr_(array->at(index));
  if(index == 0) {
    /* -- first item */
    stack.push_back({&curr_, open_, close_, 0});
    printTop(formatter_, true);
  }
  else {
    assert(!stack.empty());

    /* -- level(s) up from the tree */
    const auto* top_(&stack.back());
    if(top_->mark->level + 1 != curr_.level) {
      printTop(formatter_, false);
      stack.pop_back();
      assert(!stack.empty());
      return true;
    }

    /* -- level down into the tree */
    stack.push_back({&curr_, open_, close_, top_->indent + 1});
    printTop(formatter_, true);
  }

  /* -- move to next one */
  ++index;

  return true;
}

TestMarkPrinter::TestMarkPrinter(
    const std::vector<TestMark::LinearizedRecord>* array_,
    int& index_) :
  pimpl(new Impl(array_, index_)) {

}

TestMarkPrinter::~TestMarkPrinter() {
  odelete(pimpl);
}

bool TestMarkPrinter::printLine(
    TestMarkFormatter& formatter_) {
  return pimpl->handleItem(
      formatter_, &TestMarkFormatter::openMark, &TestMarkFormatter::closeMark);
}

bool TestMarkPrinter::printDeleted(
    TestMarkFormatter& formatter_) {
  return pimpl->handleItem(
      formatter_, &TestMarkFormatter::openDeleted, &TestMarkFormatter::closeDeleted);
}

bool TestMarkPrinter::printAdded(
    TestMarkFormatter& formatter_) {
  return pimpl->handleItem(
      formatter_, &TestMarkFormatter::openAdded, &TestMarkFormatter::closeAdded);
}

bool TestMarkPrinter::skipLine(
    TestMarkFormatter& formatter_) {
  return pimpl->handleItem(formatter_, nullptr, nullptr);
}

} /* namespace OTest2 */

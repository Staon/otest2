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
#include <iostream>
#include <sstream>

#include <utils.h>

namespace OTest2 {

struct TestMarkPrinter::Impl {
    const std::vector<TestMark::LinearizedRecord>* array;
    struct StackRecord {
        const TestMark::LinearizedRecord* mark;
        std::string prefix;
        int indent;
        bool skip;
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
        std::ostream& os_,
        bool open_);
    bool handleItem(
        std::ostream& os_,
        const std::string& prefix_,
        bool skip_);
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
    std::ostream& os_,
    bool open_) {
  assert(!stack.empty());

  auto& top_(stack.back());
  if(!top_.skip) {
    std::ostringstream oss_;
    oss_ << top_.prefix;
    for(int i_(0); i_ < top_.indent; ++i_)
      oss_ << "  ";
    if(open_) {
      if(!top_.mark->label.empty())
        oss_ << top_.mark->label << ": ";
      top_.mark->me->printOpen(os_, oss_.str());
    }
    else
      top_.mark->me->printClose(os_, oss_.str());
  }
}

bool TestMarkPrinter::Impl::handleItem(
    std::ostream& os_,
    const std::string& prefix_,
    bool skip_) {
  assert(array != nullptr);

  /* -- out of the end */
  if(index >= array->size()) {
    /* -- finish all opened nodes */
    if(!stack.empty()) {
      printTop(os_, false);
      stack.pop_back();
      return true;
    }

    /* -- empty stack, empty input sequence => final end */
    return false;
  }

  auto& curr_(array->at(index));
  if(index == 0) {
    /* -- first item */
    stack.push_back({&curr_, prefix_, 0, skip_});
    printTop(os_, true);
  }
  else {
    assert(!stack.empty());

    /* -- level(s) up from the tree */
    auto* top_(&stack.back());
    if(top_->mark->level + 1 != curr_.level) {
      printTop(os_, false);
      stack.pop_back();
      assert(!stack.empty());
      return true;
    }

    /* -- level down into the tree */
    stack.push_back({&curr_, prefix_, top_->indent + 1, skip_});
    printTop(os_, true);
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
    std::ostream& os_,
    const std::string& prefix_) {
  return pimpl->handleItem(os_, prefix_, false);
}

bool TestMarkPrinter::skipLine(
    std::ostream& os_) {
  return pimpl->handleItem(os_, "", true);
}

} /* namespace OTest2 */

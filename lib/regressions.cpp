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

#include <otest2/regressions.h>

#include <assert.h>
#include <sstream>
#include <string>

#include <context.h>
#include <difflogblock.h>
#include <objectpath.h>
#include <testmark.h>
#include <testmarkdiffprinter.h>
#include <testmarkptr.h>
#include <testmarkstorage.h>

namespace OTest2 {

bool RegressionAssertion::compareObjectMark(
    const std::string& key_,
    TestMarkPtr test_mark_) {
  assert(test_mark_ != nullptr);

  const Context& context_(otest2Context());

  /* -- get test mark from the storage */
  const std::string full_key_(context_.object_path->getRegressionKey(key_));
  TestMarkPtr stored_(context_.test_mark_storage->getTestMark(full_key_));

  /* -- compare the marks */
  bool equal_(false);
  if(stored_ != nullptr)
    equal_ = test_mark_->isEqual(*stored_);

  /* -- report the assertion */
  std::ostringstream oss_;
  if(equal_)
    oss_ << "regression check '" << full_key_ << "' has passed";
  else
    oss_ << "regression check '" << full_key_ << "' has failed";
  enterAssertion(equal_, oss_.str(), false);

  /* -- report the difference if the check fails */
  if(!equal_) {
    /* -- print current test mark */
    oss_.str("");
    test_mark_->printMark(oss_, "");
    assertionMessage(equal_, "  ----- Current -----");
    assertionMessage(equal_, oss_.str());

    assertionMessage(equal_, "  ----- Original -----");
    if(stored_ != nullptr) {
      /* -- print the original mark */
      oss_.str("");
      stored_->printMark(oss_, "");
      assertionMessage(equal_, oss_.str());
    }

    assertionMessage(equal_, "  ----- Difference ----");
    if(stored_ != nullptr) {
      /* -- compute the difference */
      std::vector<TestMark::LinearizedRecord> array1_;
      std::vector<TestMark::LinearizedRecord> array2_;
      DiffLogBlocks diff_log_;
      DiffLogBuilderBlock log_builder_(&diff_log_);
      test_mark_->computeDiff(*stored_, array1_, array2_, log_builder_);

      /* -- print the difference */
      oss_.str("");
      printTestMarkDiff(oss_, array1_, array2_, diff_log_, 3);
      assertionMessage(equal_, oss_.str());
    }
    else {
      /* -- the new mark is one big addition */
      oss_.str("");
      test_mark_->printMark(oss_, "+ ");
      assertionMessage(equal_, oss_.str());
    }
  }

  return leaveAssertion(equal_);
}

bool RegressionAssertion::storeObjectMark(
    const std::string& key_,
    TestMarkPtr test_mark_) {
  assert(test_mark_ != nullptr);

  const Context& context_(otest2Context());

  /* -- store the mark */
  const std::string full_key_(context_.object_path->getRegressionKey(key_));
  context_.test_mark_storage->setTestMark(full_key_, test_mark_);

  /* -- report the assertion */
  std::ostringstream oss_;
  oss_ << "stored regression mark '" << full_key_ << "'";
  enterAssertion(false, oss_.str(), false);
  return leaveAssertion(false);
}

} /* -- namespace OTest2 */

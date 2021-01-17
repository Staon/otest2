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

#include "filecompare.h"

#include <iomanip>
#include <string>
#include <vector>

#include <otest2/assertstream.h>
#include <otest2/difflogblock.h>
#include <otest2/hirschberg.h>

namespace OTest2 {

namespace Examples {

namespace {

void slurpFile(
    std::vector<std::string>& data_,
    std::istream& is_) {
  for(std::string line_; std::getline(is_, line_);) {
    data_.push_back(line_);
  }
}

} /* -- namespace */

bool FileCompare::testCompareFiles(
    std::istream& file_,
    std::istream& expected_) {
  /* -- read contents of the files */
  std::vector<std::string> current_data_;
  slurpFile(current_data_, file_);
  std::vector<std::string> expected_data_;
  slurpFile(expected_data_, expected_);

  /* -- compute the difference */
  DiffLogBlocks diff_log_;
  DiffLogBuilderBlock log_builder_(&diff_log_);
  hirschbergDiff(current_data_, expected_data_, log_builder_);

  bool result_(diff_log_.empty());
  AssertStream report_(enterAssertion(result_));
  if(result_) {
    /* -- There is no difference, the files match */
    report_ << "OK" << commitMsg();
  }
  else {
    /* -- the files are different */
    report_ << "the file is different than the expected one" << commitMsg();

    /* -- print the difference */
    for(const auto& difference_ : diff_log_) {
      for(int i_(difference_.left_begin); i_ < difference_.left_end; ++i_) {
        report_ << foreground(Color::GREEN) << std::setfill('0')
            << std::setw(4) << (i_ + 1) << "     : + " << current_data_[i_]
            << resetAttrs() << commitMsg();
      }

      for(int i_(difference_.right_begin); i_ < difference_.right_end; ++i_) {
        report_ << foreground(Color::RED) << std::setfill('0')
            << "     " << std::setw(4) << (i_ + 1) << ": - " << expected_data_[i_]
            << resetAttrs() << commitMsg();
      }
    }
  }
  return report_.getResult();
}

} /* -- namespace Examples */

} /* -- namespace OTest2 */

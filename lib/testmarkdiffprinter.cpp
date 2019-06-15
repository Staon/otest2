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
#include <otest2/testmarkdiffprinter.h>

#include <iostream>

#include <otest2/testmarkprinter.h>

namespace OTest2 {

void printTestMarkDiff(
    std::ostream& os_,
    const std::vector<TestMark::DiffRecord>& left_,
    const std::vector<TestMark::DiffRecord>& right_,
    const DiffLogBlocks& diff_log_,
    int context_) {
  int left_line_(0);
  int right_line_(0);
  TestMarkPrinter left_printer_(&left_, left_line_);
  TestMarkPrinter right_printer_(&right_, right_line_);
  bool trailing_context_(false);

  for(const auto& diff_ : diff_log_) {
    /* -- trailing context of previous change */
    if(trailing_context_) {
      for(int beg_(left_line_);
          left_line_ < beg_ + context_ && left_line_ < diff_.left_begin;
          )
        left_printer_.printLine(os_, "  ");
    }
    else
      trailing_context_ = true;

    /* -- skip unchanged lines */
    if(left_line_ < diff_.left_begin - context_)
      os_ << "  ......... \n";
    for(; left_line_ < diff_.left_begin - context_;)
      left_printer_.skipLine(os_);
    for(; right_line_ < diff_.right_begin;)
      right_printer_.skipLine(os_);

    /* -- print the context lines */
    for(; left_line_ < diff_.left_begin;)
      left_printer_.printLine(os_, "  ");

    /* -- print the changes */
    for(; right_line_ < diff_.right_end;)
      right_printer_.printLine(os_, "- ");
    for(; left_line_ < diff_.left_end;)
      left_printer_.printLine(os_, "+ ");
  }

  /* -- print trailing context */
  if(trailing_context_) {
    bool gap_mark_(true);
    for(int beg_(left_line_); left_line_ < beg_ + context_;) {
      if(!left_printer_.printLine(os_, "  ")) {
        gap_mark_ = false;
        break;
      }
    }
    if(gap_mark_)
      os_ << "  ......... \n";
  }
}

} /* namespace OTest2 */

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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKDIFFPRINTER_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKDIFFPRINTER_H_

#include <iosfwd>
#include <vector>

#include <otest2/difflogblock.h>
#include <otest2/testmark.h>

namespace OTest2 {

/**
 * @brief Print difference of two testmarks
 *
 * @param os_ An output stream
 * @param left_ The left testmark
 * @param right_ The right testmark
 * @param diff_log_ The difference of the marks
 * @param context_ Count of lines printed as a context of each change
 */
void printTestMarkDiff(
    std::ostream& os_,
    const std::vector<TestMark::LinearizedRecord>& left_,
    const std::vector<TestMark::LinearizedRecord>& right_,
    const DiffLogBlocks& diff_log_,
    int context_);

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TESTMARKDIFFPRINTER_H_ */

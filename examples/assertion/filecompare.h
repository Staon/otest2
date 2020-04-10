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

#ifndef OTest2__EXAMPLES_ASSERTION_FILECOMPARE_H_
#define OTest2__EXAMPLES_ASSERTION_FILECOMPARE_H_

#include <iosfwd>

#include <otest2/assertcontext.h>
#include <otest2/assertionannotation.h>

namespace OTest2 {

namespace Examples {

/**
 * @brief Comparison of two files
 *
 * This is an example class showing how to implement custom assertion
 */
class FileCompare : public AssertContext {
  public:
    /* -- avoid copying */
    FileCompare(
        const FileCompare&) = delete;
    FileCompare& operator = (
        const FileCompare&) = delete;

    /* -- inherit the constructor - the parent constructor is invoked
     *    from the generated code. */
    using AssertContext::AssertContext;

    bool testCompareFiles(
        std::istream& file_,
        std::istream& expected_);
};

/**
 * @brief The file comparison assertion
 *
 * This assertion compares content of two files. It fails if the contents
 * don't equal.
 *
 * @param file_ Current file
 * @param expected_ Expected file
 * @return True if the files are the same
 */
bool testFileCompare(
    std::istream& file_,
    std::istream& expected_) TEST_ASSERTION_MARK(::OTest2::Examples::FileCompare, testCompareFiles);

} /* -- namespace Examples */

} /* -- namespace OTest2 */

#endif /* -- OTest2__EXAMPLES_ASSERTION_FILECOMPARE_H_ */

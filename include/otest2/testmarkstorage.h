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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKSTORAGE_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKSTORAGE_H_

#include <string>

#include <otest2/testmarkptr.h>

namespace OTest2 {

class TestMarkFactory;

/**
 * @brief Storage of test marks
 */
class TestMarkStorage {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     *
     * @param factory_ A factory of storage marks. The ownership is not taken.
     * @param storage_file_ Name of the storage file
     */
    explicit TestMarkStorage(
        TestMarkFactory* factory_,
        const std::string& storage_file_);

    /**
     * @brief Dtor
     */
    virtual ~TestMarkStorage();

    /* -- avoid copying */
    TestMarkStorage(
        const TestMarkStorage&) = delete;
    TestMarkStorage& operator = (
        const TestMarkStorage&) = delete;

    /**
     * @brief Set new or reset old test mark
     *
     * @param key_ Key of the test mark
     * @param test_mark_ The test mark
     */
    void setTestMark(
        const std::string& key_,
        TestMarkPtr test_mark_);

    /**
     * @brief Get test mark
     *
     * @param key_ Key of the test mark
     * @return The test mark or null pointer if the key is not found.
     */
    TestMarkPtr getTestMark(
        const std::string& key_) const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TESTMARKSTORAGE_H_ */

/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2
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

#ifndef OTest2_INCLUDE_OTEST2_TAGSSTACK_H_
#define OTest2_INCLUDE_OTEST2_TAGSSTACK_H_

#include <string>
#include <vector>

#include <otest2/tags.h>

namespace OTest2 {

/**
 * @brief Stack of assigned tags
 *
 * The tags are inherited from parent objects (test cases inherit tags
 * from the suite). This stack keeps current tag set.
 */
class TagsStack {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     */
    TagsStack();

    /**
     * @brief Dtor
     */
    ~TagsStack();

    /* -- avoid copying */
    TagsStack(
        const TagsStack&) = delete;
    TagsStack& operator = (
        const TagsStack&) = delete;

    /**
     * @brief Push tags
     *
     * @param name_ Name of the test object
     * @param tags_ The tags
     */
    void pushTags(
        const std::string& name_,
        const Tags& tags_);

    /**
     * @brief Pop tags at the top of the stack
     */
    void popTags();

    /**
     * @brief One record of the tag stack
     */
    struct TagRecord {
        std::string name;  /**< name of the test object */
        Tags tags;         /**< object's tags */
    };

    /**
     * @brief Fill the stack into a vector (used for glob evaluation)
     *
     * @param[out] tags_ The tag vector
     */
    void fillTags(
        std::vector<TagRecord>& tags_) const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_TAGSSTACK_H_ */

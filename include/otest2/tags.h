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

#ifndef OTest2_INCLUDE_OTEST2_TAGS_H_
#define OTest2_INCLUDE_OTEST2_TAGS_H_

#include <initializer_list>
#include <string>
#include <vector>

namespace OTest2 {

/**
 * @brief List of tags assigned to a testing object
 */
class Tags {
  private:
    std::vector<std::string> tags;

  public:
    /**
     * @brief Ctor - empty list of tags
     */
    Tags();

    /**
     * @brief Ctor - initialized list of tags
     */
    Tags(
        std::initializer_list<std::string> initializer_);

    /**
     * @brief Copy ctor
     */
    Tags(
        const Tags& other_);

    /**
     * @brief Move ctor
     */
    Tags(
        Tags&& other_);

    /**
     * @brief Dtor
     */
    ~Tags();

    /**
     * @brief Swap contents
     */
    void swap(
        Tags& other_) noexcept;

    /**
     * @brief Assignment
     */
    Tags& operator = (
        const Tags& other_);

    /**
     * @brief Move assignment
     */
    Tags& operator = (
        Tags&& other_) noexcept;

    /**
     * @brief Append new tag
     */
    void appendTag(
        const std::string& tag_);

    /**
     * @brief Find a tag in the list
     *
     * @param tag_ The tag
     * @return True if the tag is present
     */
    bool findTag(
        const std::string& tag_) const noexcept;

    /**
     * @brief Check if the list is empty
     */
    bool isEmpty() const noexcept;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_TAGS_H_ */

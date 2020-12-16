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

#ifndef OTest2_INCLUDE_OTEST2_TAGFILTER_H_
#define OTest2_INCLUDE_OTEST2_TAGFILTER_H_

#include <string>

#include <otest2/exc.h>

namespace OTest2 {

class TagsStack;

/**
 * @brief Invalid tag expression
 */
class TagExpressionException : public Exception {
  private:
    std::string message;

  public:
    /**
     * @brief Ctor
     *
     * @param message_ An error message
     */
    explicit TagExpressionException(
        const std::string& message_);

    /**
     * @brief Ctor - invalid character in the expression
     *
     * @param location_ Location of the error
     * @param c_ The character
     */
    explicit TagExpressionException(
        int location_,
        char c_);

    /**
     * @brief Ctor - syntax error
     *
     * @param location_ Location of the error
     * @param token_ Invalid token
     */
    explicit TagExpressionException(
        int location_,
        const std::string& token_);

    /**
     * @brief Move ctor
     */
    TagExpressionException(
        TagExpressionException&& exc_);

    /* -- avoid copying */
    TagExpressionException(
        const TagExpressionException&) = delete;
    TagExpressionException& operator = (
        const TagExpressionException&) = delete;

    /* -- exception interface */
    virtual std::string reason() const override;
};

/**
 * @brief Filter of tagged testing objects
 */
class TagFilter {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Empty filter - all objects are run
     */
    TagFilter();

    /**
     * @brief Ctor
     *
     * @param tag_expression_ A tag expression
     */
    explicit TagFilter(
        const std::string& tag_expression_);

    /**
     * @brief Dtor
     */
    ~TagFilter();

    /* -- avoid copying */
    TagFilter(
        const TagFilter&) = delete;
    TagFilter& operator = (
        const TagFilter&) = delete;

    /**
     * @brief Check whether the testing object is filtered from the run
     *
     * @param tags_stack_ Stack of objects' tags
     * @return True if the tags don't match and the object is rejected
     *     from the test run.
     */
    bool filterObject(
        const TagsStack& tags_stack_) const noexcept;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_TAGFILTER_H_ */

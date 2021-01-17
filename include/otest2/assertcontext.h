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

#ifndef OTest2__INCLUDE_OTEST2_ASSERTCONTEXT_H_
#define OTest2__INCLUDE_OTEST2_ASSERTCONTEXT_H_

#include <initializer_list>
#include <string>

namespace OTest2 {

class AssertStream;
class Context;

/**
 * @brief Assertion context
 *
 * This is a helper class used for creation of assertion objects. The class
 * keeps a context needed by the implementations of the assertions.
 */
class AssertContext {
  private:
    const Context* context;
    std::string file;
    int lineno;
    std::initializer_list<const char*> parameters;

  public:
    /* -- avoid copying */
    AssertContext(
        const AssertContext&) = delete;
    AssertContext& operator =(
        const AssertContext&) = delete;

    /**
     * @brief Ctor
     *
     * @param context_ The OTest2 context
     * @param file_ filename which the assertion is located in
     * @param lineno_ line number of the assertion
     * @param parameters_ Array of strings representing the parameters
     *     of the assertion. The initializer list is valid just inside
     *     one expression as well as the instance of the assertion context
     *     is.
     */
    explicit AssertContext(
        const Context& context_,
        const std::string& file_,
        int lineno_,
        std::initializer_list<const char*> parameters_);

  protected:
    /**
     * @brief Dtor
     */
    ~AssertContext();


    /**
     * @brief Enter an assertion
     *
     * @param result_ Result of the assertion (the checked condition)
     * @return An assertion stream. This object allows to print more
     *     descriptive description why the assertion has failed including
     *     some text attributes like color or style. The object correctly
     *     closes the assertion by its destruction.
     */
    AssertStream enterAssertion(
        bool result_);

    /**
     * @brief Implementation of a simple assertion
     *
     * This method implements a simple assertion which doesn't report additive
     * messages.
     *
     * @param condition_ The checked condition
     * @param message_ The assertion message (it may be empty)
     * @param print_expression_ If it's true, the first assertion parameter
     *     is printed with the assertion message.
     * @return the condition
     */
    bool simpleAssertionImpl(
        bool condition_,
        const std::string& message_,
        bool print_expression_);

    /**
     * @brief Get the OTest2 context
     */
    const Context& otest2Context() const;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTCONTEXT_H_ */

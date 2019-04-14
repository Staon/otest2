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

#include <string>

namespace OTest2 {

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
    std::string expression;

  public:
    /* -- avoid copying */
    AssertContext(
        const AssertContext&) = delete;
    AssertContext& operator =(
        const AssertContext&) = delete;

  public:
    /**
     * @brief Ctor
     *
     * @param context_ The OTest2 context
     * @param file_ filename which the assertion is located in
     * @param lineno_ line number of the assertion
     * @param expression_ the checked expression as a string. Use empty
     *     string if there is no strigifized expression.
     */
    explicit AssertContext(
        const Context& context_,
        const std::string& file_,
        int lineno_,
        const std::string expression_);

  protected:
    /**
     * @brief Dtor
     */
    ~AssertContext();

    /**
     * @brief Generic assertion function
     *
     * This is a generic function used by assertion implementations
     *
     * @param condition_ The checked condition
     * @param message_ The assertion message (it may be empty)
     * @param use_expression_ If it's true, a textual representation of
     *     the first assertion parameter is added into the assertion message.
     * @return the condition
     */
    bool testAssertImpl(
        bool condition_,
        const std::string& message_,
        bool use_expression_) const;

    /**
     * @brief Get the OTest2 context
     */
    const Context& otest2Context() const;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTCONTEXT_H_ */

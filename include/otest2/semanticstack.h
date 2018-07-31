/*
 * Copyright (C) 2018 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_SEMANTICSTACK_H_
#define OTest2__INCLUDE_OTEST2_SEMANTICSTACK_H_

namespace OTest2 {

/**
 * @brief Semantic stack
 *
 * This is a stack of return values. The stack is used to return values
 * from cases and suites.
 */
class SemanticStack {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    SemanticStack(
        const SemanticStack&) = delete;
    SemanticStack& operator =(
        const SemanticStack&) = delete;

    /**
     * @brief Ctor
     */
    SemanticStack();

    /**
     * @brief Dtor
     */
    ~SemanticStack();

    /**
     * @brief Push a value
     */
    void push(
        bool value_);

    /**
     * @brief Get top value
     */
    bool top() const;

    /**
     * @brief Set top value
     */
    void setTop(
        bool value_);

    /**
     * @brief Pop top value
     */
    void pop();

    /**
     * @brief AND last two values and replace them by the result
     */
    void popAnd();
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_SEMANTICSTACK_H_ */

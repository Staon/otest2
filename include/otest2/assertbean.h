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

#ifndef OTest2__INCLUDE_OTEST2_ASSERTBEAN_H_
#define OTest2__INCLUDE_OTEST2_ASSERTBEAN_H_

#include <string>

namespace OTest2 {

/**
 * @brief A helper class packing assertion condition and an assertion message
 *     together.
 */
class AssertBean {
  private:
    bool condition;
    std::string message;

  public:
    /**
     * @brief Default ctor - false condition, empty message
     */
    AssertBean();

    /**
     * @brief Ctor
     *
     * @param condition_ Assertion condition
     * @param message_ Assertion message
     */
    explicit AssertBean(
        bool condition_,
        const std::string& message_);

    /**
     * @brief Copy ctor
     */
    AssertBean(
        const AssertBean& src_);

    /**
     * @brief Dtor
     */
    ~AssertBean();

    /**
     * @brief Swap contents
     */
    void swap(
        AssertBean& b2_) noexcept;

    /**
     * @brief Copy operator
     */
    AssertBean& operator = (
        const AssertBean& src_);

    /**
     * @brief Get assertion condition
     */
    bool getCondition() const;

    /**
     * @brief Get assertion message
     */
    const std::string& getMessage() const;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_ASSERTBEAN_H_ */

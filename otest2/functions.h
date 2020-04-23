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

#ifndef OTest2__OTEST2_FUNCTIONS_H_
#define OTest2__OTEST2_FUNCTIONS_H_

#include "function.h"

#include <memory>
#include <vector>

namespace OTest2 {

class Functions;
typedef std::shared_ptr<Functions> FunctionsPtr;

/**
 * @brief A container keeping testing function specifications
 */
class Functions {
  private:
    FunctionsPtr prev_level;
    std::vector<FunctionPtr> functions;

  public:
    /**
     * @brief Ctor
     *
     * @param prev_level_ Pointer to parent scope. It can be null for the root
     *     scope.
     */
    explicit Functions(
        FunctionsPtr prev_level_);

    /**
     * @brief Dtor
     */
    ~Functions();

    /* -- avoid copying */
    Functions(
        const Functions&) = delete;
    Functions& operator = (
        const Functions&) = delete;

    /**
     * @brief Get pointer to parent scope
     */
    FunctionsPtr getPrevLevel() const;

    /**
     * @brief Append a function
     *
     * @param function_ The function
     */
    void appendFunction(
        FunctionPtr function_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_FUNCTIONS_H_ */

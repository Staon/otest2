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

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace OTest2 {

class Functions;
typedef std::shared_ptr<Functions> FunctionsPtr;

/**
 * @brief A container keeping fixture functions (pairs of start-up and
 *     tear-down functions)
 */
class Functions {
  private:
    FunctionsPtr prev_level;
    std::vector<FunctionPtr> start_ups;
    std::vector<FunctionPtr> tear_downs;

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
     * @brief Append a fixture functions
     *
     * @param start_up_ A start-up function of the fixture
     * @param tear_down_ A tear-down function of the fixture
     */
    void appendFixture(
        FunctionPtr start_up_,
        FunctionPtr tear_down_);

    /**
     * @brief Generate function marshalers
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     * @param classname_ Name of the testing class which the marshaler is
     *     generated in.
     */
    void generateMarshalers(
        std::ostream& os_,
        int indent_,
        const std::string& classname_) const;

    /**
     * @brief Generate registration of the functions
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     * @param regfce_ Registration function
     * @param classname_ Name of the class which the marshalers are generated
     *     in.
     */
    void generateRegistration(
        std::ostream& os_,
        int indent_,
        const std::string& classname_) const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_FUNCTIONS_H_ */

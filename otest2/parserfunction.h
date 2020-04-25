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

#ifndef OTest2__OTEST2_PARSERFUNCTION_H_
#define OTest2__OTEST2_PARSERFUNCTION_H_

#include <utility>

namespace clang {

class FunctionDecl;

} /* -- namespace clang */

namespace OTest2 {

class ParserContext;

namespace Parser {

struct FunctionFlags {
    bool start_up;
    bool tear_down;
    bool test_state;
    bool first_state;

    explicit FunctionFlags(
        bool test_state_);
};

/**
 * @brief Check whether the function declaration can be a part of the fixtures
 *     block
 *
 * @param context_ The parser context
 * @param fce_ The function
 * @return True if the function is just declaration which can be part of
 *     the fixtures block
 */
bool isAllowedFunctionDeclaration(
    ParserContext* context_,
    clang::FunctionDecl* fce_);

/**
 * @brief Parse a function of a testing object (suite, case)
 *
 * @param[in] context_ The parser context
 * @param[in] function_ The function's AST node
 * @param[in,out] fce_flags_ Flags of functions to be parsed
 * @return A pair of flags: <!error, test state>
 */
std::pair<bool, bool> parseFunction(
    ParserContext* context_,
    clang::FunctionDecl* function_,
    FunctionFlags& fce_flags_);

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSERFUNCTION_H_ */

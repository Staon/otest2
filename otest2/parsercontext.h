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

#ifndef OTest2__OTEST2_PARSER_CONTEXT_H_
#define OTest2__OTEST2_PARSER_CONTEXT_H_

#include <string>

#include "location.h"

namespace clang {

class ASTContext;
class LangOptions;
class SourceLocation;
class SourceManager;
class SourceRange;

} /* -- namespace clang */

namespace OTest2 {

class Generator;
class ParserException;

struct ParserContext {
    /* -- error reporting */
    bool* failure;
    ParserException* exception;

    /* -- parser state */
    enum State {
      ROOT = 0,
      SUITE_FIXTURES,
      SUITE_FUNCTIONS,
      SUITE_CASES,
      CASE_FIXTURES,
      CASE_FUNCTIONS,
      CASE_STATES,
    };
    State state;
    clang::ASTContext* comp_context;
    clang::SourceManager* srcmgr;
    clang::LangOptions* langopts;

    /* -- output generation */
    Generator* generator;
    Location last_location;

    explicit ParserContext(
        clang::ASTContext* comp_context_,
        clang::SourceManager* srcmgr_,
        clang::LangOptions* langopts_,
        Generator* generator_,
        bool* failure_,
        ParserException* exception_);

    Location createLocation(
        const clang::SourceLocation& loc_);
    template<typename Node_>
    clang::SourceRange getNodeRange(
        Node_* node_);
    template<typename Node_>
    void setError(
        const std::string& message_,
        Node_* node_);
    template<typename Node_>
    void copyInput(
        Node_* node_,
        bool end_);
    template<typename Node_>
    void moveToEnd(
        Node_* node_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSER_CONTEXT_H_ */

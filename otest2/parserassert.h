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

#ifndef OTest2__OTEST2_PARSERASSERT_H_
#define OTest2__OTEST2_PARSERASSERT_H_

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <utility>

#include "generator.h"
#include "location.h"
#include "parsercontext.h"

namespace OTest2 {

namespace Parser {

struct AnnotationRegex;

class AssertVisitor : public clang::RecursiveASTVisitor<AssertVisitor> {
  public:
    typedef clang::RecursiveASTVisitor<AssertVisitor> Parent;

  private:
    ParserContext* context;
    Location current;
    bool allow_sections;

    bool generateAssertion(
        clang::CallExpr* expr_,
        clang::Decl* declref_,
        clang::FunctionDecl* fce_,
        std::string class_arg_,
        std::string method_arg_,
        std::vector<std::string> template_params_,
        std::vector<Generator::AssertionArg> args_ranges_,
        int ignored_params_);

    bool visitAssertion(
        clang::CallExpr* expr_,
        clang::Decl* declref_,
        clang::FunctionDecl* fce_,
        const AnnotationRegex& cmp_);
    bool visitAssertionExpr(
        clang::CallExpr* expr_,
        clang::Decl* declref_,
        clang::FunctionDecl* fce_,
        const AnnotationRegex& cmp_);
    bool visitSwitchState(
        clang::CallExpr* expr_,
        clang::Decl* declref_,
        clang::FunctionDecl* fce_);

  public:
    explicit AssertVisitor(
        ParserContext* context_,
        const Location& curr_,
        bool allow_sections_);
    virtual ~AssertVisitor();

    bool VisitCallExpr(
        clang::CallExpr* expr_);
    bool TraverseCXXTryStmt(
        clang::CXXTryStmt* stmt_);
    bool TraverseIfStmt(
        clang::IfStmt* stmt_);

    Location getCurrentLocation() const;
};

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSERASSERT_H_ */

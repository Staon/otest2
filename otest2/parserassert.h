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

#include "location.h"
#include "parsercontext.h"

namespace OTest2 {

namespace Parser {

class AssertVisitor : public clang::RecursiveASTVisitor<AssertVisitor> {
  private:
    ParserContext* context;
    Location current;
    clang::Stmt* subtree_root;

  public:
    explicit AssertVisitor(
        ParserContext* context_,
        const Location& curr_);
    virtual ~AssertVisitor();

    bool VisitCallExpr(
        clang::CallExpr* expr_);
    bool VisitCXXTryStmt(
        clang::CXXTryStmt* stmt_);
    bool dataTraverseStmtPre(
        clang::Stmt* stmt_);
    bool dataTraverseStmtPost(
        clang::Stmt* stmt_);

    Location getCurrentLocation() const;
};

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSERASSERT_H_ */

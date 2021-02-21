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
#include "parsercode.h"

#include <clang/AST/Stmt.h>

#include "generator.h"
#include "location.h"
#include "parserassert.h"
#include "parsercontextimpl.h"

namespace OTest2 {

namespace Parser {

bool parseCodeBlock(
    ParserContext* context_,
    clang::Stmt* stmt_,
    bool allow_sections_) {
  /* -- get source range of the block */
  clang::SourceRange range_(context_->getNodeRange(stmt_));
  Location begin_(context_->createLocation(range_.getBegin()));
  Location end_(context_->createLocation(range_.getEnd()));

  /* -- open the user area */
  context_->generator->startUserArea(begin_);

  /* -- visit the block (manipulate all assertions) */
  AssertVisitor visitor_(context_, begin_, allow_sections_);
  visitor_.TraverseStmt(stmt_);
  if(*context_->failure)
    return false;

  /* -- finish rest of the block after last assertion */
  context_->generator->copySource(
      visitor_.getCurrentLocation(),
      context_->createLocation(range_.getEnd()));

  /* -- close the user area */
  context_->generator->endUserArea(end_);

  return true;
}

clang::VarDecl* getVarDecl(
    ParserContext* context_,
    clang::Stmt* stmt_) {
  /* -- check the declaration statement */
  if(!clang::isa<clang::DeclStmt>(stmt_)) {
    context_->setError("invalid declaration statement", stmt_);
    return nullptr;
  }

  /* -- there must be exactly one variable */
  clang::DeclStmt* declstmt_(clang::cast<clang::DeclStmt>(stmt_));
  if(declstmt_->decl_begin() == declstmt_->decl_end()
     || !clang::isa<clang::VarDecl>(*declstmt_->decl_begin())
     || (declstmt_->decl_begin() + 1) != declstmt_->decl_end()) {
    context_->setError("invalid declaration statement", declstmt_);
    return nullptr;
  }
  return clang::cast<clang::VarDecl>(*declstmt_->decl_begin());
}

clang::Stmt* getFunctionBody(
    ParserContext* context_,
    clang::FunctionDecl* fce_) {
  if(!fce_->doesThisDeclarationHaveABody()) {
    context_->setError("function doesn't have a body", fce_);
    return nullptr;
  }
  return fce_->getBody();
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

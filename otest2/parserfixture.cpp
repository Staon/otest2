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
#include "parserfixture.h"

#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/Stmt.h>
#include <string>

#include "generator.h"
#include "parsercontextimpl.h"

namespace OTest2 {

namespace Parser {

bool parseVariable(
    ParserContext* context_,
    clang::VarDecl* vardecl_) {
  std::string varname_(vardecl_->getNameAsString());
  std::string type_(vardecl_->getType().getAsString());

  clang::Expr* init_(vardecl_->getInit());
  if(init_ == nullptr) {
    /* -- no initializer */
    context_->generator->appendVariable(varname_, type_);
  }
  else {
    /* -- variable with initializer */
    if(vardecl_->getInitStyle() != clang::VarDecl::CallInit) {
      context_->setError("only the callinit is supported", vardecl_);
      return false;
    }

    /* -- expression with cleanups is a wrapper, I nest into it. */
    if(clang::isa<clang::ExprWithCleanups>(init_)) {
      init_ = clang::cast<clang::ExprWithCleanups>(init_)->getSubExpr();
    }

    /* -- get range of the expression */
    clang::SourceRange range_(context_->getNodeRange(init_));

//    std::cout << init_->getStmtClassName() << std::endl;

    /* -- adjust the range for constructor call */
    if(clang::isa<clang::CXXConstructExpr>(init_)) {
      clang::CXXConstructExpr* ctrexpr_(
          clang::cast<clang::CXXConstructExpr>(init_));
      const int argnum_(ctrexpr_->getNumArgs());

      /* -- find first and last non-default argument */
      int last_index_(-1);
      if(argnum_ > 0) {
        for(int i_(0); i_ < argnum_; ++i_) {
          clang::Expr *arg_(ctrexpr_->getArg(i_));
          if(clang::isa<clang::CXXDefaultArgExpr>(arg_))
            break;
          last_index_ = i_;
        }
      }

      /* -- get source range of the arguments */
      if(last_index_ < 0) {
        range_ = clang::SourceRange(range_.getEnd(), range_.getEnd());
      }
      else {
        clang::SourceRange first_(context_->getNodeRange(ctrexpr_->getArg(0)));
        clang::SourceRange last_(
            context_->getNodeRange(ctrexpr_->getArg(last_index_)));
        range_ = clang::SourceRange(first_.getBegin(), last_.getEnd());
      }
    }

    /* -- notify the generator */
    context_->generator->appendVariableInit(
        varname_,
        type_,
        context_->createLocation(range_.getBegin()),
        context_->createLocation(range_.getEnd()));
  }

  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

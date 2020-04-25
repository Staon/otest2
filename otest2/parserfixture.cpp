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
#include "parserannotationimpl.h"
#include "parsercontextimpl.h"

#include <iostream>

namespace OTest2 {

namespace Parser {

bool parseVariable(
    ParserContext* context_,
    clang::VarDecl* vardecl_) {
  std::string varname_(vardecl_->getNameAsString());
  std::string type_(vardecl_->getType().getAsString());

  /* -- check the user data annotation */
  AnnotationRegex annotation_(USER_DATA_VAR_ANNOTATION);
  bool user_data_(hasAnnotation(vardecl_, annotation_));

  /* -- Get the initializer. Default-initialized class is handled as
   *    CxxConstructExpr without parameters. */
  clang::Expr* init_(vardecl_->getInit());
  clang::SourceRange init_range_;
  if(init_ != nullptr) {
    /* -- just the C++ call initializer is supported */
    if(vardecl_->getInitStyle() != clang::VarDecl::CallInit) {
      context_->setError("only the callinit (C++98) initializer is supported", vardecl_);
      return false;
    }

    /* -- an expression with cleanups is a wrapper, I nest into it. */
    if(clang::isa<clang::ExprWithCleanups>(init_)) {
      init_ = clang::cast<clang::ExprWithCleanups>(init_)->getSubExpr();
    }

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

      if(last_index_ < 0) {
        /* -- there are no arguments or all of the arguments are defaulted */
        init_ = nullptr;
      }
      else {
        /* -- get source range of the arguments */
        clang::SourceRange first_(context_->getNodeRange(ctrexpr_->getArg(0)));
        clang::SourceRange last_(
            context_->getNodeRange(ctrexpr_->getArg(last_index_)));
        init_range_ = clang::SourceRange(first_.getBegin(), last_.getEnd());
      }
    }
    else {
      /* -- something different than constructor call */
      init_range_ = context_->getNodeRange(init_);
    }
  }

  if(init_ == nullptr) {
    /* -- no initializer */
    if(user_data_) {
      std::string key_(annotation_.matches[1].str());
      if(key_.empty())
        key_ = varname_;
      context_->generator->appendUserData(varname_, key_, type_);
    }
    else
      context_->generator->appendVariable(varname_, type_);
  }
  else {
    /* -- a variable with initializer */

    /* -- User data are initialized from the OTest2 context. There must not be
     *    a custom initializer. */
    if(user_data_) {
      context_->setError("the user data variable must not be initialized", vardecl_);
      return false;
    }

    /* -- notify the generator */
    context_->generator->appendVariableInit(
        varname_,
        type_,
        context_->createLocation(init_range_.getBegin()),
        context_->createLocation(init_range_.getEnd()));
  }

  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

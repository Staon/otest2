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
#include "parsercase.h"

#include <clang/AST/Decl.h>
#include <memory>

#include "function.h"
#include "generator.h"
#include "objecttags.h"
#include "parserannotation.h"
#include "parsercode.h"
#include "parsercontextimpl.h"
#include "parserfixture.h"
#include "parserfunction.h"
#include "parsertags.h"

namespace OTest2 {

namespace Parser {

bool parseCaseBody(
    ParserContext* context_,
    clang::NamespaceDecl* ns_) {
  FunctionFlags fce_flags_(true);

  enum class State {
      FIXTURES,
      FUNCTIONS,
      STATES,
  } state_(State::FIXTURES);
  auto iter_(ns_->decls_begin());
  while(iter_ != ns_->decls_end()) {
    switch(state_) {
      case State::FIXTURES: {
        /* -- case variable */
        if(clang::isa<clang::VarDecl>(*iter_)) {
          auto vardecl_(clang::cast<clang::VarDecl>(*iter_));
          if(!parseVariable(context_, vardecl_))
            return false;

          ++iter_;
          continue;
        }

        /* -- functions of the case */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          if(isAllowedFunctionDeclaration(context_, fce_)) {
            /* -- The function doesn't stop the fixtures block. The forward
             *    declarations of functions are ignored as they are not
             *    needed in the generated code anymore. */
            ++iter_;
            continue;
          }

          /* -- finish the fixture block */
          context_->generator->finishCaseFixtures();

          /* -- enter the function block */
          state_ = State::FUNCTIONS;
          continue;
        }

        context_->setError("Invalid case item", *iter_);
        return false;
      }

      case State::FUNCTIONS: {
        /* -- start-up, tear-down and user functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          auto result_(parseFunction(context_, fce_, fce_flags_));
          if(!result_.first)
            return false;
          if(result_.second)
            state_ = State::STATES;

          ++iter_;
          continue;
        }

        context_->setError("Invalid case item", *iter_);
        return false;
      }

      case State::STATES: {
        /* -- state functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          if(!parseFunction(context_, fce_, fce_flags_).first)
            return false;

          ++iter_;
          continue;
        }

        context_->setError("Invalid case item", *iter_);
        return false;
      }

      default:
        assert(false);
        context_->setError("Invalid case item", *iter_);
        return false;
    }
  }

  /* -- correctly finish object which is not complete */
  switch(state_) {
    case State::FIXTURES:
      context_->generator->finishCaseFixtures();
      [[fallthrough]] /* -- missing break is expected */
    case State::FUNCTIONS:
      context_->generator->finishCaseFunctions();
      context_->generator->emptyState();
      [[fallthrough]]
    case State::STATES:
      break;
    default:
      context_->setError("invalid format of the case", ns_);
      return false;
  }

  return true;
}

bool parseCase(
    ParserContext* context_,
    clang::NamespaceDecl* ns_) {
  /* -- parse case's tags */
  ObjectTags tags_;
  if(!parseTags(context_, ns_, tags_))
    return false;

  context_->generator->enterCase(ns_->getNameAsString(), tags_);
  if(!parseCaseBody(context_, ns_))
    return false;
  context_->generator->leaveCase();

  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

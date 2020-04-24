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
#include "parserannotation.h"
#include "parsercode.h"
#include "parsercontextimpl.h"
#include "parserfixture.h"
#include "parserfunction.h"

namespace OTest2 {

namespace Parser {

bool parseCaseBody(
    ParserContext* context_,
    clang::NamespaceDecl* ns_) {
  FunctionFlags fce_flags_(true);

  for(
      auto iter_(ns_->decls_begin());
      iter_ != ns_->decls_end();
      ++iter_) {
    switch(context_->state) {
      case ParserContext::CASE_FIXTURES: {
        /* -- case variable */
        if(clang::isa<clang::VarDecl>(*iter_)) {
          auto vardecl_(clang::cast<clang::VarDecl>(*iter_));
          if(!parseVariable(context_, vardecl_))
            return false;
          continue;
        }

        /* -- start up or tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          context_->generator->finishCaseFixtures();

          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          auto result_(parseFunction(context_, fce_, fce_flags_));
          if(!result_.first)
            return false;
          if(result_.second)
            context_->state = ParserContext::CASE_STATES;
          else
            context_->state = ParserContext::CASE_FUNCTIONS;
          continue;
        }

        context_->setError("Invalid case item", *iter_);
        return false;
      }

      case ParserContext::CASE_FUNCTIONS: {
        /* -- tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          auto result_(parseFunction(context_, fce_, fce_flags_));
          if(!result_.first)
            return false;
          if(result_.second)
            context_->state = ParserContext::CASE_STATES;
          continue;
        }

        context_->setError("Invalid case item", *iter_);
        return false;
      }

      case ParserContext::CASE_STATES: {
        /* -- tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          if(!parseFunction(context_, fce_, fce_flags_).first)
            return false;
          continue;
        }

        context_->setError("Invalid case item", *iter_);
        return false;
      }

      default:
        context_->setError("Invalid case item", *iter_);
        return false;
    }
  }

  /* -- handle empty case */
  switch(context_->state) {
    case ParserContext::CASE_FIXTURES:
      context_->generator->finishCaseFixtures();
      /* -- missing break is expected */
    case ParserContext::CASE_FUNCTIONS: {
      context_->generator->finishCaseFunctions();

      /* -- generate empty state object */
      const std::string name_("AnonymousState");
      FunctionPtr function_(std::make_shared<Function>(name_));
      context_->generator->enterState(name_, function_);
      context_->generator->emptyBody();
      context_->generator->leaveState();

      break;
    }
    case ParserContext::CASE_STATES:
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
  context_->state = ParserContext::CASE_FIXTURES;

  context_->generator->enterCase(ns_->getNameAsString());
  if(!parseCaseBody(context_, ns_))
    return false;
  context_->generator->leaveCase();

  context_->state = ParserContext::SUITE_CASES;

  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

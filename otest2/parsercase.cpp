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

#include "generator.h"
#include "parserannotation.h"
#include "parsercode.h"
#include "parsercontextimpl.h"
#include "parserfixture.h"

namespace OTest2 {

namespace Parser {

bool parseCaseBody(
    ParserContext* context_,
    clang::NamespaceDecl* ns_) {

  for(
      auto iter_(ns_->decls_begin());
      iter_ != ns_->decls_end();
      ++iter_) {
    switch(context_->state) {
      case ParserContext::CASE_BEGIN: {
        /* -- case variable */
        if(clang::isa<clang::VarDecl>(*iter_)) {
          auto vardecl_(clang::cast<clang::VarDecl>(*iter_));
          if(!parseVariable(context_, vardecl_))
            return false;
          continue;
        }

        /* -- start up or tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          /* -- skip empty declarations */
          if(!fce_->isThisDeclarationADefinition())
            continue;
          auto body_(getFunctionBody(context_, fce_));

          /* -- suite start up */
          if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
            context_->state = ParserContext::CASE_TEAR_DOWN;
            context_->generator->caseStartUp();
            if(body_ == nullptr) {
              context_->setError("the startUp event must be defined!", fce_);
              return false;
            }
            if(!parseCodeBlock(context_, body_))
              return false;
            continue;
          }

          /* -- suite tear up */
          if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
            context_->state = ParserContext::CASE_STATES;
            context_->generator->caseStartUp();
            context_->generator->emptyBody();
            context_->generator->caseTearDown();
            if(body_ == nullptr) {
              context_->setError("the tearDown event must be defined!", fce_);
              return false;
            }
            if(!parseCodeBlock(context_, body_))
              return false;
            continue;
          }

          /* -- state function */
          if(hasAnnotation(fce_, STATE_ANNOTATION)) {
            context_->state = ParserContext::CASE_STATES;
            context_->generator->caseStartUp();
            context_->generator->emptyBody();
            context_->generator->caseTearDown();
            context_->generator->emptyBody();
            if(body_ != nullptr) {
              context_->generator->enterState(fce_->getNameAsString());
              if(!parseCodeBlock(context_, body_))
                return false;
              context_->generator->leaveState();
            }
            continue;
          }

          context_->setError("case functions are not supported yet!", fce_);
          return false;
        }

        context_->setError("Invalid case item", *iter_);
        return false;
      }

      case ParserContext::CASE_TEAR_DOWN: {
        /* -- tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          auto body_(getFunctionBody(context_, fce_));
          if(body_ == nullptr)
            return false;

          /* -- suite start up */
          if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
            context_->setError("the start-up function is already defined", fce_);
            return false;
          }

          /* -- suite tear up */
          if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
            context_->state = ParserContext::CASE_STATES;
            context_->generator->caseTearDown();
            if(body_ == nullptr) {
              context_->setError("the tearDown event must be defined!", fce_);
              return false;
            }
            if(!parseCodeBlock(context_, body_))
              return false;
            continue;
          }

          /* -- state function */
          if(hasAnnotation(fce_, STATE_ANNOTATION)) {
            context_->state = ParserContext::CASE_STATES;
            context_->generator->caseTearDown();
            context_->generator->emptyBody();
            if(body_ != nullptr) {
              context_->generator->enterState(fce_->getNameAsString());
              if(!parseCodeBlock(context_, body_))
                return false;
              context_->generator->leaveState();
            }
            continue;
          }

          context_->setError("case functions are not supported yet!", fce_);
          return false;
        }

        context_->setError("Invalid case item", *iter_);
        return false;
      }

      case ParserContext::CASE_STATES: {
        /* -- tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          auto body_(getFunctionBody(context_, fce_));
          if(body_ == nullptr)
            return false;

          /* -- suite start up */
          if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
            context_->setError("the start-up function is already defined", fce_);
            return false;
          }

          /* -- suite tear up */
          if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
            context_->setError("the tear-down function is already defined", fce_);
            return false;
          }

          /* -- state function */
          if(hasAnnotation(fce_, STATE_ANNOTATION)) {
            if(body_ != nullptr) {
              context_->generator->enterState(fce_->getNameAsString());
              if(!parseCodeBlock(context_, body_))
                return false;
              context_->generator->leaveState();
            }
            continue;
          }

          context_->setError("case functions are not supported yet!", fce_);
          return false;
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
    case ParserContext::CASE_BEGIN:
      context_->generator->caseStartUp();
      context_->generator->emptyBody();
      /* -- missing break is expected */
    case ParserContext::CASE_TEAR_DOWN:
      context_->generator->caseTearDown();
      context_->generator->emptyBody();
      break;
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
  context_->state = ParserContext::CASE_BEGIN;

  context_->generator->enterCase(ns_->getNameAsString());
  if(!parseCaseBody(context_, ns_))
    return false;
  context_->generator->leaveCase();

  context_->state = ParserContext::SUITE_CASES;

  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

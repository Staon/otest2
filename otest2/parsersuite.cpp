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
#include "parsersuite.h"

#include "generator.h"
#include "parserannotation.h"
#include "parsercase.h"
#include "parsercode.h"
#include "parsercontextimpl.h"
#include "parserfixture.h"
#include "parserfunction.h"

namespace OTest2 {

namespace Parser {

SuiteVisitor::SuiteVisitor(
    ParserContext* context_) :
  context(context_) {

}

SuiteVisitor::~SuiteVisitor() {

}

bool parseSuiteBody(
    ParserContext* context_,
    clang::NamespaceDecl* ns_) {
  FunctionFlags fce_flags_(false);

  for(
      auto iter_(ns_->decls_begin());
      iter_ != ns_->decls_end();
      ++iter_) {
    switch(context_->state) {
      case ParserContext::SUITE_FIXTURES: {
        /* -- suite variable */
        if(clang::isa<clang::VarDecl>(*iter_)) {
          auto vardecl_(clang::cast<clang::VarDecl>(*iter_));
          if(!parseVariable(context_, vardecl_))
            return false;
          continue;
        }

        /* -- suite functions - start-up, tear-down and user functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          if(isAllowedFunctionDeclaration(context_, fce_)) {
            /* -- The function doesn't stop the fixtures block. The forward
             *    declarations of functions are ignored as they are not
             *    needed in the generated code anymore. */
            continue;
          }
          else {
            context_->generator->finishSuiteFixtures();
            context_->state = ParserContext::SUITE_FUNCTIONS;
            if(!parseFunction(context_, fce_, fce_flags_).first)
              return false;
            continue;
          }
        }

        /* -- test case */
        if(clang::isa<clang::NamespaceDecl>(*iter_)) {
          auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
          if(hasAnnotation(casens_, CASE_ANNOTATION)) {
            context_->generator->finishSuiteFixtures();
            context_->generator->finishSuiteFunctions();
            if(!parseCase(context_, casens_))
              return false;
            continue;
          }
        }

        context_->setError("Invalid suite item", *iter_);
        return false;
      }

      case ParserContext::SUITE_FUNCTIONS: {
        /* -- tear down functions */
        if(clang::isa<clang::FunctionDecl>(*iter_)) {
          auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
          if(!parseFunction(context_, fce_, fce_flags_).first)
            return false;
          continue;
        }

        /* -- test case */
        if(clang::isa<clang::NamespaceDecl>(*iter_)) {
          auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
          if(hasAnnotation(casens_, CASE_ANNOTATION)) {
            context_->generator->finishSuiteFunctions();
            if(!parseCase(context_, casens_))
              return false;
            continue;
          }
        }

        context_->setError("Invalid suite item", *iter_);
        return false;
      }

      case ParserContext::SUITE_CASES: {
        /* -- test case */
        if(clang::isa<clang::NamespaceDecl>(*iter_)) {
          auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
          if(hasAnnotation(casens_, CASE_ANNOTATION)) {
            if(!parseCase(context_, casens_))
              return false;
            continue;
          }
        }

        context_->setError("Invalid suite item", *iter_);
        return false;
      }

      default:
        context_->setError("Invalid suite item", *iter_);
        return false;
    }
  }

  /* -- handle empty suite */
  switch(context_->state) {
    case ParserContext::SUITE_FIXTURES:
      context_->generator->finishSuiteFixtures();
      /* -- missing break is expected */
    case ParserContext::SUITE_FUNCTIONS:
      context_->generator->finishSuiteFunctions();
      break;
    case ParserContext::SUITE_CASES:
      break;
    default:
      context_->setError("invalid format of the suite", ns_);
      return false;
  }

  return true;
}

bool parseSuite(
    ParserContext* context_,
    clang::NamespaceDecl* ns_) {
  /* -- copy the input file */
  context_->copyInput(ns_, false);

  /* -- enter the suite */
  std::string suitename_(ns_->getNameAsString());
  context_->generator->enterSuite(suitename_);

  /* -- parse body of the suite */
  context_->state = ParserContext::SUITE_FIXTURES;
  if(!parseSuiteBody(context_, ns_))
    return false;
  context_->state = ParserContext::ROOT;

  /* -- leave the suite */
  context_->generator->leaveSuite();

  /* -- skip the body of the function */
  context_->moveToEnd(ns_);

  return true;
}

bool SuiteVisitor::VisitNamespaceDecl(
    clang::NamespaceDecl* ns_) {
  if(hasAnnotation(ns_, SUITE_ANNOTATION)) {
    if(!parseSuite(context, ns_))
      return false;
  }

  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

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

    for(
        auto iter_(ns_->decls_begin());
        iter_ != ns_->decls_end();
        ++iter_) {
      switch(context_->state) {
        case ParserContext::SUITE_BEGIN: {
          /* -- suite variable */
          if(clang::isa<clang::VarDecl>(*iter_)) {
            auto vardecl_(clang::cast<clang::VarDecl>(*iter_));
            if(!parseVariable(context_, vardecl_))
              return false;
            continue;
          }

          /* -- start up or tear down functions */
          if(clang::isa<clang::FunctionDecl>(*iter_)) {
            auto fce_(clang::cast<clang::FunctionDecl>(*iter_));
            auto body_(getFunctionBody(context_, fce_));
            if(body_ == nullptr)
              return false;

            /* -- suite start up */
            if(hasAnnotation(fce_, START_UP_ANNOTATION)) {
              context_->state = ParserContext::SUITE_TEAR_DOWN;
              context_->generator->suiteStartUp();
              if(!parseCodeBlock(context_, body_))
                return false;
              continue;
            }

            /* -- suite tear down */
            if(hasAnnotation(fce_, TEAR_DOWN_ANNOTATION)) {
              context_->state = ParserContext::SUITE_CASES;
              context_->generator->suiteStartUp();
              context_->generator->emptyBody();
              context_->generator->suiteTearDown();
              if(!parseCodeBlock(context_, body_))
                return false;
              continue;
            }

            context_->setError("suite functions are not supported yet!", fce_);
            return false;
          }

          /* -- test case */
          if(clang::isa<clang::NamespaceDecl>(*iter_)) {
            auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
            if(hasAnnotation(casens_, CASE_ANNOTATION)) {
              context_->state = ParserContext::CASE_BEGIN;
              context_->generator->suiteStartUp();
              context_->generator->emptyBody();
              context_->generator->suiteTearDown();
              context_->generator->emptyBody();
              context_->generator->enterCase(casens_->getNameAsString());
              if(!parseCaseBody(context_, casens_))
                return false;
              context_->generator->leaveCase();
              context_->state = ParserContext::SUITE_CASES;
              continue;
            }
          }

          context_->setError("Invalid suite item", *iter_);
          return false;
        }

        case ParserContext::SUITE_TEAR_DOWN: {
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
              context_->state = ParserContext::SUITE_CASES;
              context_->generator->suiteTearDown();
              if(!parseCodeBlock(context_, body_))
                return false;
              continue;
            }

            context_->setError("suite functions are not supported yet!", fce_);
            return false;
          }

          /* -- test case */
          if(clang::isa<clang::NamespaceDecl>(*iter_)) {
            auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
            if(hasAnnotation(casens_, CASE_ANNOTATION)) {
              context_->state = ParserContext::CASE_BEGIN;
              context_->generator->suiteTearDown();
              context_->generator->emptyBody();
              context_->generator->enterCase(casens_->getNameAsString());
              if(!parseCaseBody(context_, casens_))
                return false;
              context_->generator->leaveCase();
              context_->state = ParserContext::SUITE_CASES;
              continue;
            }
            /* -- test case */
          }

          context_->setError("Invalid suite item", *iter_);
          return false;
        }

        case ParserContext::SUITE_CASES: {
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

            context_->setError("suite functions are not supported yet!", fce_);
            return false;
          }

          /* -- test case */
          if(clang::isa<clang::NamespaceDecl>(*iter_)) {
            auto casens_(clang::cast<clang::NamespaceDecl>(*iter_));
            if(hasAnnotation(casens_, CASE_ANNOTATION)) {
              context_->state = ParserContext::CASE_BEGIN;
              context_->generator->enterCase(casens_->getNameAsString());
              if(!parseCaseBody(context_, casens_))
                return false;
              context_->generator->leaveCase();
              context_->state = ParserContext::SUITE_CASES;
              continue;
            }
            /* -- test case */
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
      case ParserContext::SUITE_BEGIN:
        context_->generator->suiteStartUp();
        context_->generator->emptyBody();
        /* -- missing break is expected */
      case ParserContext::SUITE_TEAR_DOWN:
        context_->generator->suiteTearDown();
        context_->generator->emptyBody();
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
  context_->state = ParserContext::SUITE_BEGIN;
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

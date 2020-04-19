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

#ifndef OTest2__OTEST2_PARSERSUITE_H_
#define OTest2__OTEST2_PARSERSUITE_H_

#include <clang/AST/RecursiveASTVisitor.h>

namespace clang {

class NamespaceDecl;

} /* -- namespace clang */

namespace OTest2 {

class ParserContext;

namespace Parser {

bool parseSuite(
    ParserContext* context_,
    clang::NamespaceDecl* ns_);
bool parseSuiteBody(
    ParserContext* context_,
    clang::NamespaceDecl* ns_);

class SuiteVisitor : public clang::RecursiveASTVisitor<SuiteVisitor> {
  private:
    ParserContext* context;

  public:
    explicit SuiteVisitor(
        ParserContext* context_);
    virtual ~SuiteVisitor();

    bool VisitNamespaceDecl(
        clang::NamespaceDecl* ns_);
};

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSERSUITE_H_ */

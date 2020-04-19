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
#include "parsercontext.h"

#include <clang/AST/ASTContext.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>

#include "excparser.h"
#include "generator.h"

#ifndef OTest2__OTEST2_PARSERCONTEXTIMPL_H_
#define OTest2__OTEST2_PARSERCONTEXTIMPL_H_

namespace OTest2 {

template<typename Node_>
clang::SourceRange ParserContext::getNodeRange(
    Node_* node_) {
  clang::SourceRange range_(node_->getSourceRange());

  /* -- adjust range at the end of last token */
  clang::SourceLocation true_end_(
      clang::Lexer::getLocForEndOfToken(
          range_.getEnd(),
          0,
          *srcmgr,
          *langopts));

  return clang::SourceRange(range_.getBegin(), true_end_);
}

template<typename Node_>
void ParserContext::setError(
    const std::string& message_,
    Node_* node_) {
  /* -- get error location */
  clang::SourceLocation beg_(node_->getLocStart());
  clang::SourceLocation end_(node_->getLocEnd());
  clang::PresumedLoc pbeg_(srcmgr->getPresumedLoc(beg_));
  clang::PresumedLoc pend_(srcmgr->getPresumedLoc(end_));

  /* -- fill the exception */
  *failure = true;
  exception->setException(
      message_,
      pbeg_.getFilename(),
      pbeg_.getLine(),
      pbeg_.getColumn(),
      pend_.getLine(),
      pend_.getColumn());
}

template<typename Node_>
void ParserContext::copyInput(
    Node_* node_,
    bool end_) {
  /* -- get node's location */
  clang::SourceRange range_(getNodeRange(node_));
  clang::SourceLocation loc_;
  if(!end_)
    loc_ = range_.getBegin();
  else
    loc_ = range_.getEnd();
  Location l2_(createLocation(loc_));

  /* -- get content of the input file */
  if(last_location < l2_) {
    generator->startUserArea(last_location);
    generator->copySource(last_location, l2_);
    generator->endUserArea(l2_);
  }
}

template<typename Node_>
void ParserContext::moveToEnd(
    Node_* node_) {
  clang::SourceRange range_(getNodeRange(node_));
  last_location = createLocation(range_.getEnd());
}

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSERCONTEXTIMPL_H_ */

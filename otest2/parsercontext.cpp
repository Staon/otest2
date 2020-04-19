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

#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

#include "location.h"

namespace OTest2 {

ParserContext::ParserContext(
    clang::ASTContext* comp_context_,
    clang::SourceManager* srcmgr_,
    clang::LangOptions* langopts_,
    Generator* generator_,
    bool* failure_,
    ParserException* exception_) :
  failure(failure_),
  exception(exception_),
  state(ROOT),
  comp_context(comp_context_),
  srcmgr(srcmgr_),
  langopts(langopts_),
  generator(generator_),
  last_location() {

}

Location ParserContext::createLocation(
    const clang::SourceLocation& loc_) {
  clang::PresumedLoc ploc_(srcmgr->getPresumedLoc(loc_));
  return Location(ploc_.getLine(), ploc_.getColumn());
}

} /* -- namespace OTest2 */

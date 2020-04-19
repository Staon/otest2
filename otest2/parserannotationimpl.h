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

#ifndef OTest2__OTEST2_PARSERANNOTATIONIMPL_H_
#define OTest2__OTEST2_PARSERANNOTATIONIMPL_H_

#include "parserannotation.h"

#include <clang/AST/Attr.h>
#include <clang/AST/DeclBase.h>
#include <clang/Basic/AttrKinds.h>

namespace OTest2 {

namespace Parser {

template<typename Compare_>
bool hasAnnotation(
    clang::Decl* decl_,
    Compare_& cmp_) {
  if(decl_->hasAttrs()) {
    for(
        auto iter_(decl_->attr_begin());
        iter_ != decl_->attr_end();
        ++iter_) {
      if((*iter_)->getKind() == clang::attr::Annotate) {
        auto sa_(clang::cast<clang::AnnotateAttr>(*iter_));
        auto str_(sa_->getAnnotation());
        if(cmp_(str_.str())) {
          return true;
        }
      }
    }
  }
  return false;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSERANNOTATIONIMPL_H_ */

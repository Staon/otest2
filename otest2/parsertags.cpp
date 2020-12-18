/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2
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
#include "parsertags.h"

#include <clang/AST/Decl.h>
#include <iostream>
#include <sstream>

#include "parserannotationimpl.h"
#include "parsercontextimpl.h"

namespace OTest2 {

namespace Parser {

bool parseTags(
    ParserContext* context_,
    const clang::Decl* decl_,
    ObjectTags& tags_) {
  /* -- get the annotation */
  AnnotationRegex annotation_cmp_(TAG_ANNOTATION);
  if(!hasAnnotation(decl_, annotation_cmp_))
    return true; /* -- no annotation, no error */

  /* -- parse the annotation string */
  std::string tags_string_(annotation_cmp_.matches[1].str());
  std::regex split_("(,)");
  std::sregex_token_iterator iter_(
      tags_string_.begin(), tags_string_.end(), split_, -1);
  std::regex tag_name_("[a-zA-Z_][a-zA-Z0-9_.\\-]*");
  for(; iter_ != std::sregex_token_iterator(); ++iter_) {
    /* -- check the tag name */
    const std::string& tag_((*iter_).str());
    if(!std::regex_match(tag_, tag_name_)) {
      std::ostringstream oss_;
      oss_ << "name '" << tag_ << "' is not a valid tag name";
      context_->setError(oss_.str(), decl_);
      return false;
    }
    tags_.insert(tag_);
  }

  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

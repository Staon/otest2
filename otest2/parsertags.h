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

#ifndef OTest2_OTEST2_PARSERTAGS_H_
#define OTest2_OTEST2_PARSERTAGS_H_

#include <set>
#include <string>

#include "objecttags.h"

namespace clang {

class Decl;

} /* -- namespace clang */

namespace OTest2 {

class ParserContext;

namespace Parser {

/**
 * @brief Parse tags
 *
 * @param[in] context_ Parser context
 * @param[in] decl_ A declaration object. The tag annotation is searched on.
 * @param[out] tags_ A container which the tags are filled in
 * @return True if the tags are correctly parsed. False if an error happens.
 */
bool parseTags(
    ParserContext* context_,
    const clang::Decl* decl_,
    ObjectTags& tags_);

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2_OTEST2_PARSERTAGS_H_ */

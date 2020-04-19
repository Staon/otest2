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

#ifndef OTest2__OTEST2_PARSERANNOTATION_H_
#define OTest2__OTEST2_PARSERANNOTATION_H_

#include <regex>
#include <string>

namespace clang {

class Decl;

} /* -- namespace clang */

namespace OTest2 {

namespace Parser {

extern const std::string SUITE_ANNOTATION;
extern const std::string CASE_ANNOTATION;
extern const std::string STATE_ANNOTATION;
extern const std::string START_UP_ANNOTATION;
extern const std::string TEAR_DOWN_ANNOTATION;
extern const std::string ASSERTION_ANNOTATION;
extern const std::string SWITCH_STATE_ANNOTATION;
extern const std::string CATCH_ANNOTATION;

template<typename Compare_>
bool hasAnnotation(
    clang::Decl* decl_,
    Compare_& cmp_);

struct AnnotationStringEqual {
    std::string ref;
    bool operator()(
        const std::string& annotation_);
};

struct AnnotationRegex {
    std::regex expr;
    std::smatch matches;

    AnnotationRegex(
        const std::string& expr_);
    bool operator()(
        const std::string& annotation_);
};

bool hasAnnotation(
    clang::Decl* decl_,
    const std::string& annotation_);

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSERANNOTATION_H_ */

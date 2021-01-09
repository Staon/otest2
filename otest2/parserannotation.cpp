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
#include "parserannotationimpl.h"

namespace OTest2 {

namespace Parser {

const std::string SUITE_ANNOTATION("otest2::suite");
const std::string CASE_ANNOTATION("otest2::case");
const std::string STATE_ANNOTATION("otest2::state");
const std::string START_UP_ANNOTATION("otest2::startUp");
const std::string TEAR_DOWN_ANNOTATION("otest2::tearDown");
const std::string ASSERTION_ANNOTATION("^otest2::assertion[(](.*)[)]$");
const std::string SWITCH_STATE_ANNOTATION("otest2::switchState");
const std::string CATCH_ANNOTATION("otest2::catch");
const std::string USER_DATA_VAR_ANNOTATION("^otest2::userData[(](.*)[)]$");
const std::string TAG_ANNOTATION("^otest2::tags[(](.*)[)]$");

bool AnnotationStringEqual::operator()(
    const std::string& annotation_) {
  return annotation_ == ref;
}

AnnotationRegex::AnnotationRegex(
    const std::string& expr_) :
  expr(expr_) {

}

bool AnnotationRegex::operator()(
    const std::string& annotation_) {
  return std::regex_match(annotation_, matches, expr);
}

bool AnnotationAny::operator()(
    const std::string& annotation_) {
  return true;
}

bool hasAnnotation(
    const clang::Decl* decl_,
    const std::string& annotation_) {
  AnnotationStringEqual cmp_{annotation_};
  return hasAnnotation(decl_, cmp_);
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

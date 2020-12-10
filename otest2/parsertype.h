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

#ifndef OTest2__OTEST2_PARSERTYPE_H_
#define OTest2__OTEST2_PARSERTYPE_H_

#include <functional>
#include <string>

namespace clang {

class CXXMethodDecl;
class CXXRecordDecl;
class NamedDecl;
class QualType;

} /* -- namespace clang */

namespace OTest2 {

class ParserContext;

namespace Parser {

/**
 * @brief Get string representation of a type
 *
 * @param context_ Parser context
 * @param type_ The type
 * @return The string representation
 */
std::string parseType(
    ParserContext* context_,
    const clang::QualType& type_);

/**
 * @brief Get string representation of a type
 *
 * @param context_ Parser context
 * @param decl_ Declaration of the type
 * @return The string representation
 */
std::string parseType(
    ParserContext* context_,
    const clang::NamedDecl& decl_);

/**
 * @brief Traverse methods of a class
 *
 * @param context_ Parser context
 * @param object_ The class declaration
 * @param post_order_ If it's true the @a object_ is traversed after
 *     the base classes.
 * @param fce_ A functor. If the functor returns true, the traversion
 *     is stopped.
 * @return True if the method is found (the @a fce_ has returned true)
 */
bool traverseMethods(
    ParserContext* context_,
    const clang::CXXRecordDecl* object_,
    bool post_order_,
    std::function<bool(const clang::CXXMethodDecl*)> fce_);

/**
 * @brief Traverse a class and all parent classes
 *
 * @param context_ Parser context
 * @param object_ The class declaration
 * @param post_order_ If it's true the @a object_ is traversed after
 *     the base classes.
 * @param fce_ A functor. If the functor returns true, the traversion
 *     is stopped.
 * @return True if the class is found (the @a fce_ has returned true)
 */
bool traverseClasses(
    ParserContext* context_,
    const clang::CXXRecordDecl* object_,
    bool post_order_,
    std::function<bool(const clang::CXXRecordDecl*)> fce_);

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_PARSERTYPE_H_ */

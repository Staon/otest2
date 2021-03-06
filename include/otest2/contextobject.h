/*
 * Copyright (C) 2018 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_CONTEXTOBJECT_H_
#define OTest2__INCLUDE_OTEST2_CONTEXTOBJECT_H_

#include <string>

namespace OTest2 {

class AssertBean;
class Context;

/**
 * @brief A generic interface of objects which offers OTest2 context
 */
class ContextObject {
  public:
    /* -- avoid copying */
    ContextObject(
        const ContextObject&) = delete;
    ContextObject& operator =(
        const ContextObject&) = delete;

    /**
     * @brief Ctor
     */
    ContextObject();

    /**
     * @brief Dtor
     */
    virtual ~ContextObject();

    /**
     * @brief Get the OTest2 context
     */
    virtual const Context& otest2Context() const = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CONTEXTOBJECT_H_ */

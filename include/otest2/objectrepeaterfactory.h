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

#ifndef OTest2_INCLUDE_OTEST2_OBJECTREPEATERFACTORY_H_
#define OTest2_INCLUDE_OTEST2_OBJECTREPEATERFACTORY_H_

#include <otest2/objectrepeaterfactoryptr.h>
#include <otest2/objectrepeaterptr.h>

namespace OTest2 {

class Context;

/**
 * @brief Factory of object repeater objects
 */
class ObjectRepeaterFactory {
  public:
    /**
     * @brief Ctor
     */
    ObjectRepeaterFactory();

    /**
     * @brief Dtor
     */
    virtual ~ObjectRepeaterFactory();

    /* -- avoid copying */
    ObjectRepeaterFactory(
        const ObjectRepeaterFactory&) = delete;
    ObjectRepeaterFactory& operator = (
        const ObjectRepeaterFactory&) = delete;

    /**
     * @brief Create repeater object
     *
     * @param context_ The OTest2 context
     * @return The repeater object
     */
    virtual ObjectRepeaterPtr createRepeater(
        const Context& context_) const = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_OBJECTREPEATERFACTORY_H_ */

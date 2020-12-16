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

#ifndef OTest2__INCLUDE_OTEST2_CASEFACTORY_H_
#define OTest2__INCLUDE_OTEST2_CASEFACTORY_H_

#include <otest2/caserepeaterptr.h>

namespace OTest2 {

class Context;
class Tags;

/**
 * @brief Simple factory of one test case
 */
class CaseFactory {
  public:
    /* -- avoid copying */
    CaseFactory(
        const CaseFactory&) = delete;
    CaseFactory& operator =(
        const CaseFactory&) = delete;

    /**
     * @brief Ctor
     */
    CaseFactory();

    /**
     * @brief Dtor
     */
    virtual ~CaseFactory();

    /**
     * @brief Get the tags assigned to the testcase
     */
    virtual Tags getCaseTags() const = 0;

    /**
     * @brief Create the test case repeater
     *
     * This method creates an instance of the test case repeater. The repeater
     * is responsible for repeated creation of the test case. Hence, it works
     * as a kind of factory.
     *
     * For historic reasons, this class is called case factory although it's
     * a factory of the repeaters.
     *
     * @param context_ The otest2 context
     * @return The test case repeater
     */
    virtual CaseRepeaterPtr createCase(
        const Context& context_) = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CASEFACTORY_H_ */

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

#ifndef OTest2__INCLUDE_OTEST2_SUITEFACTORY_H_
#define OTest2__INCLUDE_OTEST2_SUITEFACTORY_H_

#include <otest2/suiteptr.h>

namespace OTest2 {

class Context;

/**
 * @brief Simple interface for creation of a suite object
 */
class SuiteFactory {
  public:
    /* -- avoid copying */
    SuiteFactory(
        const SuiteFactory&) = delete;
    SuiteFactory& operator =(
        const SuiteFactory&) = delete;

    /**
     * @brief Ctor
     */
    SuiteFactory();

    /**
     * @brief Dtor
     */
    virtual ~SuiteFactory();

    /**
     * @brief Create the suite
     *
     * @param context_ OTest2 context
     * @return The suite
     */
    virtual SuitePtr createSuite(
        const Context& context_) = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_SUITEFACTORY_H_ */
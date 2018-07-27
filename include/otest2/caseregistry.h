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

#ifndef OTest2__INCLUDE_OTEST2_CASEREGISTRY_H_
#define OTest2__INCLUDE_OTEST2_CASEREGISTRY_H_

#include <string>

#include <otest2/casefactoryptr.h>

namespace OTest2 {

/**
 * @brief Registry of factories of test cases
 */
class CaseRegistry {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    CaseRegistry(
        const CaseRegistry&) = delete;
    CaseRegistry& operator =(
        const CaseRegistry&) = delete;

    /**
     * @brief Ctor
     */
    CaseRegistry();

    /**
     * @brief Dtor
     */
    ~CaseRegistry();

    /**
     * @brief Register a test case
     *
     * @param name_ Name of the test case
     * @param case_factory_ A factory of the test case
     */
    void registerCase(
        const std::string& name_,
        CaseFactoryPtr case_factory_);
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CASEREGISTRY_H_ */

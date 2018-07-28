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


#ifndef OTest2__INCLUDE_OTEST2_GENERUTILS_H_
#define OTest2__INCLUDE_OTEST2_GENERUTILS_H_

#include <memory>

#include <otest2/casefactory.h>
#include <otest2/caseptr.h>
#include <otest2/suitefactory.h>
#include <otest2/suiteptr.h>

namespace OTest2 {

template<typename T_>
struct TypeOfMine {
    typedef T_ Type;
};

template<typename T_>
struct TypeOfParent {
    typedef const T_& Type;
};

template<typename T_>
struct TypeOfParent<T_&> {
    typedef T_& Type;
};

template<typename Suite_>
class SuiteGeneratedFactory : public SuiteFactory {
  public:
    /* -- avoid copying */
    SuiteGeneratedFactory(
        const SuiteGeneratedFactory&) = delete;
    SuiteGeneratedFactory& operator =(
        const SuiteGeneratedFactory&) = delete;

    SuiteGeneratedFactory() {

    }

    virtual ~SuiteGeneratedFactory() {

    }

    virtual SuitePtr createSuite(
        const Context& context_) {
      return makePointer<Suite_>(context_);
    }
};

template<typename Suite_, typename Case_>
class CaseGeneratedFactory : public CaseFactory {
  private:
    Suite_* suite;
    CasePtr (Suite_::* factory_method)(const Context&);

  public:
    /* -- avoid copying */
    CaseGeneratedFactory(
        const CaseGeneratedFactory&) = delete;
    CaseGeneratedFactory& operator =(
        const CaseGeneratedFactory&) = delete;

    explicit CaseGeneratedFactory(
        Suite_* suite_,
        CasePtr (Suite_::* factory_method_)(const Context&)) :
          suite(suite_),
          factory_method(factory_method_) {

    }

    virtual ~CaseGeneratedFactory() {

    }

    virtual CasePtr createCase(
        const Context& context_) {
      return (suite->*factory_method)(context_);
    }
};

} /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_GENERUTILS_H_ */

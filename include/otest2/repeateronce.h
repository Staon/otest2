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

#ifndef OTest2_INCLUDE_OTEST2_REPEATERONCE_H_
#define OTest2_INCLUDE_OTEST2_REPEATERONCE_H_

#include <assert.h>
#include <memory>
#include <otest2/caserepeater.h>
#include <otest2/suiterepeater.h>

namespace OTest2 {

template<typename Suite_>
class SuiteRepeaterOnce : public SuiteRepeater {
  private:
    bool created;

  public:
    /* -- avoid copying */
    SuiteRepeaterOnce(
        const SuiteRepeaterOnce&) = delete;
    SuiteRepeaterOnce& operator = (
        const SuiteRepeaterOnce&) = delete;

    SuiteRepeaterOnce() :
      created(false) {

    }

    virtual ~SuiteRepeaterOnce() = default;

    virtual bool isNextRun(
        const Context& context_) const {
      return !created;
    }

    virtual std::string transformName(
        const Context& context_,
        const std::string& suitename_) const {
      assert(!created);
      return suitename_;
    }

    virtual SuitePtr createSuite(
        const Context& context_,
        const std::string& suitename_) {
      assert(!created);
      created = true;
      return makePointer<Suite_>(context_);
    }
};

template<typename Suite_, typename Case_>
class CaseRepeaterOnce : public CaseRepeater {
  public:
    typedef CasePtr (Suite_::* FactoryMethod)(const Context&);

  private:
    Suite_* suite;
    FactoryMethod factory_method;
    bool created;

  public:
    /* -- avoid copying */
    CaseRepeaterOnce(
        const CaseRepeaterOnce&) = delete;
    CaseRepeaterOnce& operator = (
        const CaseRepeaterOnce&) = delete;

    explicit CaseRepeaterOnce(
        Suite_* suite_,
        FactoryMethod factory_method_) :
      suite(suite_),
      factory_method(factory_method_),
      created(false) {
      assert(suite != nullptr && factory_method != nullptr);

    }

    virtual ~CaseRepeaterOnce() = default;

    virtual bool isNextRun(
        const Context& context_) const {
      return !created;
    }

    virtual std::string transformName(
        const Context& context_,
        const std::string& case_name_) const {
      return case_name_;
    }

    virtual CasePtr createCase(
        const Context& context_,
        const std::string& case_name_) {
      created = true;
      return (suite->*factory_method)(context_);
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_REPEATERONCE_H_ */

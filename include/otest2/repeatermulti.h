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

#ifndef OTest2_INCLUDE_OTEST2_REPEATERMULTI_H_
#define OTest2_INCLUDE_OTEST2_REPEATERMULTI_H_

#include <assert.h>
#include <functional>
#include <iostream>
#include <memory>
#include <otest2/caseptr.h>
#include <otest2/caserepeater.h>
#include <otest2/suiteptr.h>
#include <otest2/suiterepeater.h>
#include <sstream>
#include <string>

namespace OTest2 {

template<typename Suite_, typename Repeater_>
class SuiteRepeaterMulti : public SuiteRepeater {
  private:
    int index;
    std::shared_ptr<Repeater_> repeater_object;

  public:
    /* -- avoid copying */
    SuiteRepeaterMulti(
        const SuiteRepeaterMulti&) = delete;
    SuiteRepeaterMulti& operator = (
        const SuiteRepeaterMulti&) = delete;

    explicit SuiteRepeaterMulti() :
      index(1),
      repeater_object() {

    }

    virtual ~SuiteRepeaterMulti() = default;

    virtual bool isNextRun(
        const Context& context_) const override {
      /* -- first run */
      if(repeater_object == nullptr)
        return true;
      /* -- following runs */
      return repeater_object->hasNextRun(context_);
    }

    virtual std::string transformName(
        const Context& context_,
        const std::string& suite_name_) const override {
      std::ostringstream oss_;
      oss_ << suite_name_ << " (" << index << ")";
      return oss_.str();
    }

    virtual SuitePtr createSuite(
        const Context& context_,
        const std::string& case_name_) override {
      ++index;
      return makePointer<Suite_>(context_, std::ref(repeater_object));
    }
};

template<typename Suite_, typename Case_, typename Repeater_>
class CaseRepeaterMulti : public CaseRepeater {
  public:
    typedef CasePtr (Suite_::* FactoryMethod)(
        const Context&,
        std::shared_ptr<Repeater_>&);

  private:
    int index;
    std::shared_ptr<Repeater_> repeater_object;
    Suite_* suite;
    FactoryMethod factory_method;

  public:
    /* -- avoid copying */
    CaseRepeaterMulti(
        const CaseRepeaterMulti&) = delete;
    CaseRepeaterMulti& operator = (
        const CaseRepeaterMulti&) = delete;

    explicit CaseRepeaterMulti(
        Suite_* suite_,
        FactoryMethod factory_method_) :
      index(1),
      repeater_object(),
      suite(suite_),
      factory_method(factory_method_) {
      assert(suite != nullptr && factory_method != nullptr);

    }

    virtual ~CaseRepeaterMulti() = default;


    virtual bool isNextRun(
        const Context& context_) const override {
      /* -- first run */
      if(repeater_object == nullptr)
        return true;
      /* -- following runs */
      return repeater_object->hasNextRun(context_);
    }

    virtual std::string transformName(
        const Context& context_,
        const std::string& case_name_) const override {
      std::ostringstream oss_;
      oss_ << case_name_ << " (" << index << ")";
      return oss_.str();
    }

    virtual CasePtr createCase(
        const Context& context_,
        const std::string& case_name_) override {
      ++index;
      return (suite->*factory_method)(context_, repeater_object);
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_REPEATERMULTI_H_ */

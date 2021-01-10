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

#ifndef OTest2_INCLUDE_OTEST2_OBJECTREPEATERMULTIIMPL_H_
#define OTest2_INCLUDE_OTEST2_OBJECTREPEATERMULTIIMPL_H_

#include <iostream>
#include <memory>

#include <otest2/objectscenarioptr.h>
#include <otest2/objectrepeater.h>
#include <otest2/objectrepeaterfactory.h>

namespace OTest2 {

template<typename Object_, typename Repeater_>
class ObjectRepeaterMultiRoot : public ObjectRepeater {
  private:
    int index;
    std::shared_ptr<Repeater_> repeater_object;

  public:
    explicit ObjectRepeaterMultiRoot() :
      index(1),
      repeater_object() {

    }

    virtual ~ObjectRepeaterMultiRoot() = default;

    /* -- avoid copying */
    ObjectRepeaterMultiRoot(
        const ObjectRepeaterMultiRoot&) = delete;
    ObjectRepeaterMultiRoot& operator = (
        const ObjectRepeaterMultiRoot&) = delete;

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

    virtual ObjectScenarioPtr createObject(
        const Context& context_,
        const std::string& case_name_,
        ObjectPtr parent_) override {
      ++index;
      return std::make_shared<Object_>(context_, std::ref(repeater_object));
    }
};

template<typename Object_, typename Repeater_>
class ObjectRepeaterFactoryMultiRoot : public ObjectRepeaterFactory {
  public:
    ObjectRepeaterFactoryMultiRoot() = default;
    virtual ~ObjectRepeaterFactoryMultiRoot() = default;

    /* -- avoid copying */
    ObjectRepeaterFactoryMultiRoot(
        const ObjectRepeaterFactoryMultiRoot&) = delete;
    ObjectRepeaterFactoryMultiRoot& operator = (
        const ObjectRepeaterFactoryMultiRoot&) = delete;

    virtual ObjectRepeaterPtr createRepeater(
        const Context& context_) const {
      return std::make_shared<ObjectRepeaterMultiRoot<Object_, Repeater_> >();
    }
};

template<typename Parent_, typename Object_, typename Repeater_>
class ObjectRepeaterMultiNested : public ObjectRepeater {
  public:
    typedef ObjectScenarioPtr (Parent_::* FactoryMethod)(
        const Context&,
        std::shared_ptr<Repeater_>&);

  private:
    int index;
    std::shared_ptr<Repeater_> repeater_object;
    FactoryMethod factory_method;

  public:
    explicit ObjectRepeaterMultiNested(
        FactoryMethod factory_method_) :
      index(1),
      repeater_object(),
      factory_method(factory_method_) {
      assert(factory_method != nullptr);

    }

    virtual ~ObjectRepeaterMultiNested() = default;

    /* -- avoid copying */
    ObjectRepeaterMultiNested(
        const ObjectRepeaterMultiNested&) = delete;
    ObjectRepeaterMultiNested& operator = (
        const ObjectRepeaterMultiNested&) = delete;

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

    virtual ObjectScenarioPtr createObject(
        const Context& context_,
        const std::string& case_name_,
        ObjectPtr parent_) override {
      ++index;
      auto parent_typed_(std::dynamic_pointer_cast<Parent_>(parent_));
      assert(parent_typed_ != nullptr);
      return (parent_typed_.get()->*factory_method)(context_, repeater_object);
    }
};

template<typename Parent_, typename Object_, typename Repeater_>
class ObjectRepeaterFactoryMultiNested : public ObjectRepeaterFactory {
  public:
    typedef typename ObjectRepeaterMultiNested<Parent_, Object_, Repeater_>::FactoryMethod FactoryMethod;

  private:
    FactoryMethod factory_method;

  public:
    ObjectRepeaterFactoryMultiNested(
        FactoryMethod factory_method_) :
      factory_method(factory_method_) {

    }

    virtual ~ObjectRepeaterFactoryMultiNested() = default;

    /* -- avoid copying */
    ObjectRepeaterFactoryMultiNested(
        const ObjectRepeaterFactoryMultiNested&) = delete;
    ObjectRepeaterFactoryMultiNested& operator = (
        const ObjectRepeaterFactoryMultiNested&) = delete;

    virtual ObjectRepeaterPtr createRepeater(
        const Context& context_) const {
      return std::make_shared<ObjectRepeaterMultiNested<Parent_, Object_, Repeater_> >(factory_method);
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_OBJECTREPEATERMULTIIMPL_H_ */

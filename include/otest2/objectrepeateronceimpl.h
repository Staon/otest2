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

#ifndef OTest2_INCLUDE_OTEST2_OBJECTREPEATERONCEIMPL_H_
#define OTest2_INCLUDE_OTEST2_OBJECTREPEATERONCEIMPL_H_

#include <assert.h>
#include <memory>

#include <otest2/objectrepeaterfactory.h>
#include <otest2/objectrepeateronce.h>

namespace OTest2 {

template<typename Object_>
class ObjectRepeaterOnceSuite : public ObjectRepeaterOnce {
  private:
    virtual ObjectScenarioPtr doCreateObject(
        const Context& context_,
        const std::string& decorated_name_,
        ObjectPtr parent_) override {
      return std::make_shared<Object_>(context_);
    }

  public:
    ObjectRepeaterOnceSuite() = default;
    virtual ~ObjectRepeaterOnceSuite() = default;

    /* -- avoid copying */
    ObjectRepeaterOnceSuite(
        const ObjectRepeaterOnceSuite&) = delete;
    ObjectRepeaterOnceSuite& operator = (
        const ObjectRepeaterOnceSuite&) = delete;
};

template<typename Object_>
class ObjectRepeaterFactoryOnceSuite : public ObjectRepeaterFactory {
  public:
    ObjectRepeaterFactoryOnceSuite() = default;
    virtual ~ObjectRepeaterFactoryOnceSuite() = default;

    /* -- avoid copying */
    ObjectRepeaterFactoryOnceSuite(
        const ObjectRepeaterFactoryOnceSuite&) = delete;
    ObjectRepeaterFactoryOnceSuite& operator = (
        const ObjectRepeaterFactoryOnceSuite&) = delete;

    virtual ObjectRepeaterPtr createRepeater(
        const Context& context_) const {
      return std::make_shared<ObjectRepeaterOnceSuite<Object_> >();
    }
};

template<typename Parent_, typename Object_>
class ObjectRepeaterOnceCase : public ObjectRepeaterOnce {
  public:
    typedef ObjectScenarioPtr (Parent_::* FactoryMethod)(const Context&);

  private:
    FactoryMethod factory_method;

    virtual ObjectScenarioPtr doCreateObject(
        const Context& context_,
        const std::string& decorated_name_,
        ObjectPtr parent_) override {
      auto parent_typed_(std::dynamic_pointer_cast<Parent_>(parent_));
      assert(parent_typed_ != nullptr);
      return (parent_typed_.get()->*factory_method)(context_);
    }

  public:
    explicit ObjectRepeaterOnceCase(
        FactoryMethod factory_method_) :
      factory_method(factory_method_) {

    }

    virtual ~ObjectRepeaterOnceCase() = default;

    /* -- avoid copying */
    ObjectRepeaterOnceCase(
        const ObjectRepeaterOnceCase&) = delete;
    ObjectRepeaterOnceCase& operator = (
        const ObjectRepeaterOnceCase&) = delete;
};

template<typename Parent_, typename Object_>
class ObjectRepeaterFactoryOnceCase : public ObjectRepeaterFactory {
  public:
    typedef typename ObjectRepeaterOnceCase<Parent_, Object_>::FactoryMethod FactoryMethod;

  private:
    FactoryMethod factory_method;

  public:
    ObjectRepeaterFactoryOnceCase(
        FactoryMethod factory_method_) :
      factory_method(factory_method_) {

    }

    virtual ~ObjectRepeaterFactoryOnceCase() = default;

    /* -- avoid copying */
    ObjectRepeaterFactoryOnceCase(
        const ObjectRepeaterFactoryOnceCase&) = delete;
    ObjectRepeaterFactoryOnceCase& operator = (
        const ObjectRepeaterFactoryOnceCase&) = delete;

    virtual ObjectRepeaterPtr createRepeater(
        const Context& context_) const {
      return std::make_shared<ObjectRepeaterOnceCase<Parent_, Object_> >(factory_method);
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_OBJECTREPEATERONCEIMPL_H_ */

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
#include <string>

#include <otest2/objectrepeaterfactory.h>
#include <otest2/objectrepeateronce.h>

namespace OTest2 {

template<typename Object_>
class ObjectRepeaterOnceRoot : public ObjectRepeaterOnce {
  private:
    std::string section_path;

    virtual ObjectScenarioPtr doCreateObject(
        const Context& context_,
        const std::string& decorated_name_,
        ObjectPtr parent_) override {
      return std::make_shared<Object_>(context_, section_path);
    }

  public:
    explicit ObjectRepeaterOnceRoot(
        const std::string& section_path_) :
      section_path(section_path_) {

    }
    virtual ~ObjectRepeaterOnceRoot() = default;

    /* -- avoid copying */
    ObjectRepeaterOnceRoot(
        const ObjectRepeaterOnceRoot&) = delete;
    ObjectRepeaterOnceRoot& operator = (
        const ObjectRepeaterOnceRoot&) = delete;
};

template<typename Object_>
class ObjectRepeaterFactoryOnceRoot : public ObjectRepeaterFactory {
  public:
    explicit ObjectRepeaterFactoryOnceRoot() = default;
    virtual ~ObjectRepeaterFactoryOnceRoot() = default;

    /* -- avoid copying */
    ObjectRepeaterFactoryOnceRoot(
        const ObjectRepeaterFactoryOnceRoot&) = delete;
    ObjectRepeaterFactoryOnceRoot& operator = (
        const ObjectRepeaterFactoryOnceRoot&) = delete;

    virtual ObjectRepeaterPtr createRepeater(
        const Context& context_,
        const std::string& section_path_) const {
      return std::make_shared<ObjectRepeaterOnceRoot<Object_> >(section_path_);
    }
};

template<typename Parent_, typename Object_>
class ObjectRepeaterOnceNested : public ObjectRepeaterOnce {
  public:
    typedef ObjectScenarioPtr (Parent_::* FactoryMethod)(const Context&, const std::string&);

  private:
    FactoryMethod factory_method;
    std::string section_path;

    virtual ObjectScenarioPtr doCreateObject(
        const Context& context_,
        const std::string& decorated_name_,
        ObjectPtr parent_) override {
      auto parent_typed_(std::dynamic_pointer_cast<Parent_>(parent_));
      assert(parent_typed_ != nullptr);
      return (parent_typed_.get()->*factory_method)(context_, section_path);
    }

  public:
    explicit ObjectRepeaterOnceNested(
        FactoryMethod factory_method_,
        const std::string& section_path_) :
      factory_method(factory_method_),
      section_path(section_path_) {

    }

    virtual ~ObjectRepeaterOnceNested() = default;

    /* -- avoid copying */
    ObjectRepeaterOnceNested(
        const ObjectRepeaterOnceNested&) = delete;
    ObjectRepeaterOnceNested& operator = (
        const ObjectRepeaterOnceNested&) = delete;
};

template<typename Parent_, typename Object_>
class ObjectRepeaterFactoryOnceNested : public ObjectRepeaterFactory {
  public:
    typedef typename ObjectRepeaterOnceNested<Parent_, Object_>::FactoryMethod FactoryMethod;

  private:
    FactoryMethod factory_method;

  public:
    ObjectRepeaterFactoryOnceNested(
        FactoryMethod factory_method_) :
      factory_method(factory_method_) {

    }

    virtual ~ObjectRepeaterFactoryOnceNested() = default;

    /* -- avoid copying */
    ObjectRepeaterFactoryOnceNested(
        const ObjectRepeaterFactoryOnceNested&) = delete;
    ObjectRepeaterFactoryOnceNested& operator = (
        const ObjectRepeaterFactoryOnceNested&) = delete;

    virtual ObjectRepeaterPtr createRepeater(
        const Context& context_,
        const std::string& section_path_) const {
      return std::make_shared<ObjectRepeaterOnceNested<Parent_, Object_> >(factory_method, section_path_);
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_OBJECTREPEATERONCEIMPL_H_ */

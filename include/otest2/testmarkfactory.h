/*
 * Copyright (C) 2020 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKFACTORY_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKFACTORY_H_

#include <functional>
#include <memory>
#include <string>

#include <otest2/testmarkptr.h>

namespace OTest2 {

/**
 * As default constructors are special creatures, the deserialization
 * constructor must pass one parameter of type CtorMark*. The value
 * is not important (it will be null), but this constructor is different
 * to standard default constructor.
 *
 * The framework promises that only the deserialization method
 * or the destructor will be invoked just after the creation.
 */
struct CtorMark;

/**
 * @brief A factory of testmark objects used for deserialization
 */
class TestMarkFactory {
  private:
    struct Impl;
    Impl* pimpl;

    void doRegisterRecord(
        const std::string& typemark_,
        std::function<TestMarkPtr()> ctor_);

  public:
    /* -- avoid copying */
    TestMarkFactory(
        const TestMarkFactory&) = delete;
    TestMarkFactory& operator = (
        const TestMarkFactory&) = delete;

    /**
     * @brief Ctor
     *
     * The constructor automatically registers the standard testmark classes
     */
    TestMarkFactory();

    /**
     * @brief Dtor
     */
    ~TestMarkFactory();

    /**
     * @brief Create new empty testmark
     *
     * @param typemark_ Type identifier of the testmark
     * @return The testmark
     * @exception ExcTestMarkIn
     */
    TestMarkPtr createMark(
        const std::string& typemark_);

    /**
     * @brief Register a testmark type
     *
     * @tparam Mark_ The type of the registered testmark
     */
    template<typename Mark_>
    void registerMark() {
      doRegisterRecord(
          Mark_::typeMark(),
          []() {
            return std::make_shared<Mark_>(static_cast<const CtorMark*>(nullptr));
          });
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TESTMARKFACTORY_H_ */

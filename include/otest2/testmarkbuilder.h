/*
 * Copyright (C) 2019 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_TESTMARKBUILDER_H_
#define OTest2__INCLUDE_OTEST2_TESTMARKBUILDER_H_

#include <assert.h>
#include <cstdint>
#include <memory>
#include <string>

#include <otest2/testmarkptr.h>
#include <otest2/utils.h>

namespace OTest2 {

/**
 * @brief Builder of test marks
 */
class TestMarkBuilder {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     */
    TestMarkBuilder();

    /**
     * @brief Dtor
     */
    ~TestMarkBuilder();

    /* -- avoid copying */
    TestMarkBuilder(
        const TestMarkBuilder&) = delete;
    TestMarkBuilder& operator =(
        const TestMarkBuilder&) = delete;

  private:
    class Container {
      public:
        virtual ~Container() {};
        virtual void append(
            const std::string& key_,
            TestMarkPtr mark_) = 0;
        virtual TestMark* getMark() = 0;
    };

    template<typename ContainerMark_>
    class OrderedContainer : public Container {
      private:
        std::unique_ptr<ContainerMark_> container;

      public:
        explicit OrderedContainer(
            std::unique_ptr<ContainerMark_>&& container_) :
          container(std::move(container_)) {

        }
        virtual void append(
            const std::string& key_,
            TestMarkPtr mark_) {
          assert(key_.empty() && mark_ != nullptr);
          container->append(mark_);
        }
        virtual TestMark* getMark() {
          return container.release();
        }
    };

    template<typename ContainerMark_>
    class UnorderedContainer : public Container {
      private:
        std::unique_ptr<ContainerMark_> container;

      public:
        explicit UnorderedContainer(
            std::unique_ptr<ContainerMark_>&& container_) :
          container(std::move(container_)) {

        }
        virtual void append(
            const std::string& key_,
            TestMarkPtr mark_) {
          assert(!key_.empty() && mark_ != nullptr);
          container->append(key_, mark_);
        }
        virtual TestMark* getMark() {
          return container.release();
        }
    };

    template<typename ContainerMark_>
    std::unique_ptr<Container> createContainer(
        std::unique_ptr<ContainerMark_>&& container_,
        void (ContainerMark_::*)(TestMarkPtr)) {
      return std::unique_ptr<Container>(
          new OrderedContainer<ContainerMark_>(std::move(container_)));
    }

    template<typename ContainerMark_>
    std::unique_ptr<Container> createContainer(
        std::unique_ptr<ContainerMark_>&& container_,
        void (ContainerMark_::*)(const std::string&, TestMarkPtr)) {
      return std::unique_ptr<Container>(
          new UnorderedContainer<ContainerMark_>(std::move(container_)));
    }

    void openContainerImpl(
        std::unique_ptr<Container>&& container_);

  public:
    /**
     * @brief Set current key
     *
     * @param key_ The key
     */
    void setKey(
        const std::string& key_);

    /**
     * @brief Append new test mark
     *
     * @param mark_ The mark
     */
    void appendMark(
        TestMarkPtr mark_);

    /**
     * @brief Append the null mark
     */
    void appendNull();

    /**
     * @brief Append an integer mark
     *
     * @param value_ The integer value
     */
    void appendInt(
        int64_t value_);

    /**
     * @brief Append a float mark
     *
     * @param value_ The float value
     */
    void appendFloat(
        long double value_);

    /**
     * @brief Append a string mark
     *
     * @param value_ The string value
     */
    void appendString(
        const std::string& value_);

    /**
     * @brief Open a new nested level of test mark container
     *
     * @param container_ The nested container
     */
    template<typename ContainerMark_>
    void openContainer(
        std::unique_ptr<ContainerMark_>&& container_) {
      assert(container_ != nullptr);
      openContainerImpl(
          createContainer(std::move(container_), &ContainerMark_::append));
    }

    /**
     * @brief Open new nested list
     */
    void openList();

    /**
     * @brief Open new nested list
     *
     * @param prefix_ Prefix of the list
     */
    void openList(
        const std::string& prefix_);

    /**
     * @brief Open nested map
     */
    void openMap();

    /**
     * @brief Open nested map
     *
     * @param prefix_ Prefix of the map
     */
    void openMap(
        const std::string& prefix_);

    /**
     * @brief Close currently opened container
     *
     * The method closes currently opened container, pops it from the stack
     * and appends it to upper container.
     */
    void closeContainer();

    /**
     * @brief Get constructed testmark and reset the builder
     *
     * @warning The builder must be finished (all containers closed)
     */
    TestMarkPtr stealMark() const;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_TESTMARKBUILDER_H_ */

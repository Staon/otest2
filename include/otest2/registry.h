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

#ifndef OTest2__INCLUDE_REGISTRY_H_
#define OTest2__INCLUDE_REGISTRY_H_

#include <string>

#include <otest2/suitefactoryptr.h>

namespace OTest2 {

/**
 * @brief Test registry
 *
 * This class collects test suites.
 */
class Registry {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    Registry(
        const Registry&) = delete;
    Registry& operator =(
        const Registry&) = delete;

  public:
    /**
     * @brief Ctor
     */
    Registry();

    /**
     * @brief Dtor
     */
    ~Registry();

    /**
     * @brief Register a suite
     *
     * @param name_ Name of the suite
     * @param suite_factory_ A factory which creates the suite
     */
    void registerSuite(
        const std::string& name_,
        SuiteFactoryPtr suite_factory_);

    /**
     * @brief Get a suite factory at specified index
     *
     * @param index_ The index. Zero based.
     * @return The suite factory or nullptr if the index is out of the bounds.
     */
    SuiteFactoryPtr getSuite(
        int index_);

    /**
     * @brief Access of the global instance
     *
     * The global instance is used for automatic registering of suites.
     */
    static Registry& instance();
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_REGISTRY_H_ */

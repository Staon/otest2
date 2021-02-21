/*
 * Copyright (C) 2021 Ondrej Starek
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

#ifndef OTest2_INCLUDE_OTEST2_PARAMETERS_H_
#define OTest2_INCLUDE_OTEST2_PARAMETERS_H_

#include <string>

namespace OTest2 {

/**
 * @brief Generic parameters of a run of an testing object
 */
class Parameters {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    Parameters(
        const Parameters&) = delete;
    Parameters& operator = (
        const Parameters&) = delete;

    /**
     * @brief Ctor - empty parameters
     */
    Parameters();

    /**
     * @brief Move ctor
     */
    Parameters(
        Parameters&& other_) noexcept;

    /**
     * @brief Dtor
     */
    ~Parameters();

    /**
     * @brief Swap contents
     */
    void swap(
        Parameters& other_) noexcept;

    /**
     * @brief Move assignment
     */
    Parameters& operator = (
        Parameters&& other_) noexcept;

    /**
     * @brief Append new parameter
     *
     * @param name_ Name of the parameter
     * @param value_ Value
     */
    void appendParameter(
        const std::string& name_,
        const std::string& value_);

    /**
     * @brief Create one string mixed with a name of a testing object
     *
     * This method creates one string containing name of the object and
     * all parameters. Typically, the method is used in the reporters
     * to print an identifier of the object's run.
     *
     * @param Name of the testing object
     * @return Mixed string with parameters
     */
    std::string mixWithName(
        const std::string& name_) const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_PARAMETERS_H_ */

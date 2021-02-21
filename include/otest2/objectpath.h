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

#ifndef OTest2__INCLUDE_OTEST2_OBJECTPATH_H_
#define OTest2__INCLUDE_OTEST2_OBJECTPATH_H_

#include <string>

namespace OTest2 {

class Parameters;

/**
 * @brief This is a simple object keeping path to current testing object
 */
class ObjectPath {
  public:
    struct Impl;

  private:
    Impl* pimpl;

  public:
    /* -- avoid copying */
    ObjectPath(
        const ObjectPath&) = delete;
    ObjectPath& operator = (
        const ObjectPath&) = delete;

    /**
     * @brief Ctor
     */
    ObjectPath();

    /**
     * @brief Ctor with initial path
     *
     * @param full_path_ The full path (parts of path separated by '::')
     */
    explicit ObjectPath(
        const std::string& full_path_);

    /**
     * @brief Dtor
     */
    ~ObjectPath();

    /**
     * @brief Push name of currently entering testing object
     *
     * @param name_ The name
     */
    void pushName(
        const std::string& name_);

    /**
     * @brief Pop name of currently leaving testing object
     */
    void popName();

    /**
     * @brief Append parameter to current testing object
     *
     * @param name_ Name of the parameter
     * @param value_ Value of the parameter
     */
    void appendParameter(
        const std::string& name_,
        const std::string& value_);

    /**
     * @brief Get name at the top of the stack
     *
     * @return the name
     */
    std::string getCurrentName() const;

    /**
     * @brief Get parameters of current object
     *
     * @return The parameters
     */
    const Parameters& getCurrentParameters() const;

    /**
     * @brief Get path of current object
     *
     * @return The path
     */
    std::string getCurrentPath() const;

    /**
     * @brief Create key of a regression test mark according to current
     *     object path
     *
     * @param local_key_ Key of the test mark in its local scope
     * @return The regression key
     */
    std::string getRegressionKey(
        const std::string& local_key_) const;

    /**
     * @brief Tell us whether this object is a prefix of specified @a path_
     */
    bool isPrefixOf(
        const ObjectPath& path_) const noexcept;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_OBJECTPATH_H_ */

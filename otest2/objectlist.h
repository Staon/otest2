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

#ifndef OTest2_OTEST2_OBJECTLIST_H_
#define OTest2_OTEST2_OBJECTLIST_H_

#include <iosfwd>
#include <memory>
#include <string>

#include "objecttags.h"

namespace OTest2 {

namespace Parser {

class ObjectList;
typedef std::shared_ptr<ObjectList> ObjectListPtr;

/**
 * @brief List of children objects
 *
 * This method keeps a list of children objects and it can generate their
 * registration code.
 */
class ObjectList {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor - empty list
     */
    ObjectList();

    /**
     * @brief Dtor
     */
    ~ObjectList();

    /* -- avoid copying */
    ObjectList(
        const ObjectList&) = delete;
    ObjectList& operator = (
        const ObjectList&) = delete;

    /**
     * @brief Append a suite child
     *
     * @param name_ Name of the suite
     * @param tags_ Object's tags
     */
    void appendSuite(
        const std::string& name_,
        const ObjectTags& tags_);

    /**
     * @brief Append a case child
     *
     * @param name_ Name of the test case
     * @param tags_ Object's tags
     */
    void appendCase(
        const std::string& name_,
        const ObjectTags& tags_);

    /**
     * @brief Check whether the list is empty
     */
    bool isEmpty() const noexcept;

    /**
     * @brief Set repeater type of the last object in the list
     */
    void setRepeaterType(
        const std::string& repeater_type_);

    /**
     * @brief Print registrations of children object inside a suite object
     *
     * @param os_ An output stream
     * @param suite_ Name of the suite
     * @param indent_ Indentation level
     */
    void printRegistrationsInSuite(
        std::ostream& os_,
        const std::string& suite_,
        int indent_);

    /**
     * @brief Print registrations of root objects of a test source file
     *
     * @param os_ An output stream
     * @param domain_ Test domain
     * @param indent_ Indentation level
     */
    void printRegistrationsInFile(
        std::ostream& os_,
        const std::string& domain_,
        int indent_);
};

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2_OTEST2_OBJECTLIST_H_ */

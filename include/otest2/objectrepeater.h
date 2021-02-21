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

#ifndef OTest2_INCLUDE_OTEST2_OBJECTREPEATER_H_
#define OTest2_INCLUDE_OTEST2_OBJECTREPEATER_H_

#include <string>

#include <otest2/objectptr.h>
#include <otest2/objectrepeaterptr.h>
#include <otest2/objectscenarioptr.h>

namespace OTest2 {

class Context;
class ObjectPath;

/**
 * @brief Generic interface of a repeater of a testing object (this repeater
 *     is not a part of the OTest2 API)
 */
class ObjectRepeater {
  public:
    /**
     * @brief Ctor
     */
    ObjectRepeater();

    /**
     * @brief Dtor
     */
    virtual ~ObjectRepeater();

    /* -- avoid copying */
    ObjectRepeater(
        const ObjectRepeater&) = delete;
    ObjectRepeater& operator = (
        const ObjectRepeater&) = delete;

    /**
     * @brief Check whether next run of the object is planned
     *
     * @param context_ The otest2 context
     * @return True if next run is prepared
     */
    virtual bool hasNextRun(
        const Context& context_) const = 0;

    /**
     * @brief Change current object path
     *
     * This method changes current object path to distinguish different
     * runs of the testing object. Usually the some object's parameters
     * are set.
     *
     * @param[in] context_ The OTest2 context
     * @param[in,out] path_ The modified object path
     */
    virtual void modifyObjectPath(
        const Context& context_,
        ObjectPath& path_) const = 0;

    /**
     * @brief Create the testing object
     *
     * @param context_ The OTest2 context
     * @param name_ Name of the object
     * @param parent_ Parent object of the created object
     * @return The created object
     * @warning The method may be called only if the isNextRun() is true!
     */
    virtual ObjectScenarioPtr createObject(
        const Context& context_,
        const std::string& name_,
        ObjectPtr parent_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_OBJECTREPEATER_H_ */

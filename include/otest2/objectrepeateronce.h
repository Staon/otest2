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

#ifndef OTest2_INCLUDE_OTEST2_REPEATERONCE_H_
#define OTest2_INCLUDE_OTEST2_REPEATERONCE_H_

#include <string>

#include <otest2/objectrepeater.h>
#include <otest2/objectscenarioptr.h>

namespace OTest2 {

/**
 * @brief Object repeater which runs the test just once
 */
class ObjectRepeaterOnce : public ObjectRepeater {
  private:
    bool created;

    /**
     * @brief Create the object instance
     */
    virtual ObjectScenarioPtr doCreateObject(
        const Context& context_,
        const std::string& decorated_name_,
        ObjectPtr parent_) = 0;

  public:
    /**
     * @brief Ctor
     */
    ObjectRepeaterOnce();

    /**
     * @brief Dtor
     */
    virtual ~ObjectRepeaterOnce();

    /* -- avoid copying */
    ObjectRepeaterOnce(
        const ObjectRepeaterOnce&) = delete;
    ObjectRepeaterOnce& operator = (
        const ObjectRepeaterOnce&) = delete;

    /* -- repeater interface */
    virtual bool isNextRun(
        const Context& context_) const override;
    virtual std::string transformName(
        const Context& context_,
        const std::string& name_) const override;
    virtual ObjectScenarioPtr createObject(
        const Context& context_,
        const std::string& decorated_name_,
        ObjectPtr parent_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_REPEATERONCE_H_ */

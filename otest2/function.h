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

#ifndef OTest2__OTEST2_FUNCTION_H_
#define OTest2__OTEST2_FUNCTION_H_

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace OTest2 {

class VarTable;

/**
 * @brief Record of one testing function (start-up, tear-down or a test case)
 */
class Function {
  private:
    std::string name;
    struct Record {
        std::string name;
        std::string key;
        std::string type;
    };
    std::vector<Record> parameters;

    void generateFceParameters(
        std::ostream& os_,
        int indent_) const;
    void generateFceArguments(
        std::ostream& os_,
        int indent_) const;

  public:
    /**
     * @brief Ctor
     *
     * @param name_ Name of the function
     */
    explicit Function(
        const std::string& name_);

    /**
     * @brief Dtor
     */
    virtual ~Function();

    /* -- avoid copying */
    Function(
        const Function&) = delete;
    Function& operator = (
        const Function&) = delete;

    /**
     * @brief Add a parameter filled by specified user data
     *
     * @param name_ Name of the parameter
     * @param key_ Key of the user datum
     * @param type_ Type of the parameter/user datum
     */
    void addUserDataParameter(
        const std::string& name_,
        const std::string& key_,
        const std::string& type_);

    /**
     * @brief Generate marshaler of the function
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     * @param classname_ Name of the class which the marshaler is generated
     *     in.
     */
    void generateMarshaler(
        std::ostream& os_,
        int indent_,
        const std::string& classname_) const;

    /**
     * @brief Generate registration of the function
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     * @param classname_ Name of the class which the marshaler is generated
     *     in.
     */
    void generateRegistration(
        std::ostream& os_,
        int indent_,
        const std::string& classname_) const;

    /**
     * @brief Insert parameters of the function into a var table
     *
     * This method inserts function parameters into a table of object
     * variables as user data members.
     *
     * @param[out] table_ The table of variables
     */
    void enrichVarTable(
        VarTable& table_) const;
};

typedef std::shared_ptr<Function> FunctionPtr;

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_FUNCTION_H_ */

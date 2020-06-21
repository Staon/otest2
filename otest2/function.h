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

enum class FunctionAccess {
    NONE = 0,
    DOT,
    ARROW,
};

/**
 * @brief Record of one testing function (start-up, tear-down or a test case)
 */
class Function {
  private:
    FunctionAccess access;
    std::string instance;
    std::string clash;
    std::string name;
    std::string rettype;
    enum ParamType {
      USER_DATUM,
      CONTEXT,
    };
    struct Record {
        ParamType param_type;
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
    void generateInvokerArguments(
        std::ostream& os_,
        int indent_) const;

  public:
    /**
     * @brief Ctor
     *
     * @param access_ How to access the function: directly or as an instance
     *     member.
     * @param instance_ Name of the instance. May be empty for function
     *     access NONE.
     * @param clash_ A string used to resolve collisions of generated marshalers.
     *     The string is added into the marshaler name. If the string is empty,
     *     nothing is added.
     * @param classname_ Name of the class which the function is defined in.
     *     It may be empty if the access is NONE.
     * @param name_ Name of the function
     * @param rettype_ Return type of the function
     */
    explicit Function(
        FunctionAccess access_,
        const std::string& instance_,
        const std::string& clash_,
        const std::string& name_,
        const std::string& rettype_);

    /**
     * @brief Dtor
     */
    ~Function();

    /* -- avoid copying */
    Function(
        const Function&) = delete;
    Function& operator = (
        const Function&) = delete;

    /**
     * @brief Get name of the function
     */
    std::string getName() const noexcept;

    /**
     * @brief Get name of the function's invoker
     */
    std::string getInvokerName() const noexcept;

    /**
     * @brief Get typename of the invoker class
     */
    std::string getInvokerClass() const noexcept;

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
     * @brief Add a parameter passing the OTest2 context
     *
     * @param name_ Name of the parameter
     */
    void addContextParameter(
        const std::string& name_);

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
     * @brief Generate invocation of the function with different name
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     * @param fce_name_ The new function name
     */
    void generateInvocation(
        std::ostream& os_,
        int indent_,
        const std::string& fce_name_) const;

    /**
     * @brief Generate declaration of the function with different name
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     * @param fce_name_ The new function name
     */
    void generateDeclaration(
        std::ostream& os_,
        int indent_,
        const std::string& fce_name_) const;

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
     * @brief Generate an invoker object which can be passed into nested
     *     objects
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     * @param classname_ Name of the class which the invoker is generated in
     */
    void generateInvoker(
        std::ostream& os_,
        int indent_,
        const std::string& classname_) const;
};

typedef std::shared_ptr<Function> FunctionPtr;

} /* -- namespace OTest2 */

#endif /* -- OTest2__OTEST2_FUNCTION_H_ */

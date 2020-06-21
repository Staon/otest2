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

#ifndef OTest2OTEST2_VARTABLE_H_
#define OTest2OTEST2_VARTABLE_H_

#include <iosfwd>
#include <memory>
#include <string>

#include "function.h"
#include "initializer.h"

namespace OTest2 {

class VarTable;
typedef std::shared_ptr<VarTable> VarTablePtr;

/**
 * @brief Table of variables
 */
class VarTable {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     *
     * @param name_ Name of current level.
     * @param level_ Previous namespace level. The ownership is taken. It can
     *     be null.
     */
    explicit VarTable(
        const std::string& name_,
        VarTablePtr level_);

    /**
     * @brief Dtor
     */
    ~VarTable();

    /* -- avoid copying */
    VarTable(
        const VarTable&) = delete;
    VarTable& operator =(
        const VarTable&) = delete;

    /**
     * @brief Get name of the level
     *
     * @return The name
     */
    const std::string& getName() const;

    /**
     * @brief Get previous variable level
     */
    VarTablePtr getPrevLevel() const;

    /**
     * @brief Append new variable
     *
     * @param name_ Name of the variable
     * @param declaration_ Variable's declaration
     * @param initializer_ Initializer of the variable or null pointer
     */
    void appendVariable(
        const std::string& name_,
        const std::string& declaration_,
        InitializerPtr initializer_);

    /**
     * @brief Append new user datum
     *
     * @param name_ Name of the datum
     * @param key_ Key of the datum (it usually equals the name)
     * @param declaration_ Type of the datum
     */
    void appendUserData(
        const std::string& name_,
        const std::string& key_,
        const std::string& declaration_);

    /**
     * @brief Append a user function
     *
     * User functions must be passed into nested scopes so that the nested
     * testing object can invoke them.
     *
     * @param function_ The function description
     */
    void appendUserFunction(
        FunctionPtr function_);

    /**
     * @brief Append repeater variable with no initializer
     *
     * @param name_ Name of the variable
     * @param declaration_ Type of the variable
     * @param initializer_ Initializer of the repeater or null pointer
     */
    void appendRepeater(
        const std::string& name_,
        const std::string& declaration_,
        InitializerPtr initializer_);

    /**
     * @brief Print declarations
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     */
    void printDeclarations(
        std::ostream& os_,
        int indent_) const;

    /**
     * @brief Print invokers of the user functions
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     * @param classname_ Name of the class which the invoker is generated in.
     */
    void printInvokers(
        std::ostream& os_,
        int indent_,
        const std::string& classname_) const;

    /**
     * @brief Print the initializers
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     */
    void printInitializers(
        std::ostream& os_,
        int indent_) const;

    /**
     * @brief Print constructor parameters
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     */
    void printParameters(
        std::ostream& os_,
        int indent_) const;

    /**
     * @brief Print parameters of factory method (a parent method which
     *     creates current object)
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     */
    void printFactoryParameters(
        std::ostream& os_,
        int indent_) const;

    /**
     * @brief Print constructor arguments (invocation)
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     */
    void printArguments(
        std::ostream& os_,
        int indent_) const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_VARTABLE_H_ */

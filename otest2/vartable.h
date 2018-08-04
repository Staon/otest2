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
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace OTest2 {

class VarTable;
typedef std::shared_ptr<VarTable> VarTablePtr;

/**
 * @brief Table of variables
 */
class VarTable {
  private:
    std::string name;
    VarTablePtr level;
    struct Record {
        bool mine;
        std::string declaration;
        std::string initializer;
    };
    typedef std::map<std::string, Record> Variables;
    Variables variables;
    typedef std::vector<std::string> Order;
    Order order;

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
     */
    void appendVariable(
        const std::string& name_,
        const std::string& declaration_);

    /**
     * @brief Append new variable
     *
     * @param name_ Name of the variable
     * @param declaration_ Variable's declaration
     * @param initializer_ Variable's initializer
     */
    void appendVariableWithInit(
        const std::string& name_,
        const std::string& declaration_,
        const std::string& initializer_);

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

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

#ifndef OTest2OTEST2_GENERATOR_H_
#define OTest2OTEST2_GENERATOR_H_

#include <string>

namespace OTest2 {

class Location;

/**
 * @brief Generator interface
 */
class Generator {
  public:
    /**
     * @brief Ctor
     */
    Generator();

    /**
     * @brief Dtor
     */
    virtual ~Generator();

    /* -- avoid copying */
    Generator(
        const Generator&) = delete;
    Generator& operator =(
        const Generator&) = delete;

    /**
     * @brief Generate header of the file
     */
    virtual void beginFile() = 0;

    /**
     * @brief Begin of an area of user written code
     *
     * @param begin_ Location of the beginning of the area
     */
    virtual void startUserArea(
        const Location& begin_) = 0;

    /**
     * @brief Copy part of the source file
     *
     * @param begin_ Beginning of the source area
     * @param end_ End of the source area (just one character after)
     */
    virtual void copySource(
        const Location& begin_,
        const Location& end_) = 0;

    /**
     * @brief Make a test assertion
     *
     * This method dump arguments of an assertion into the output
     * stream. Just before them it inserts information about file
     * and line number.
     *
     * @param begin_ Beginning location of the assertion arguments.
     * @param end_ Ending location of the assertion arguments.
     * @param insert_expr_ If it's true, a string of containing the checked
     *     expression is inserted as an argument too.
     * @param expr_end_ Ending location of the checked expression.
     *     The expression is expected in the range <begin_, expr_end_)
     */
    virtual void makeAssertion(
        const Location& begin_,
        const Location& end_,
        bool insert_expr_,
        const Location& expr_end_) = 0;

    /**
     * @brief End of the user area
     *
     * @param end_ End of the user area
     */
    virtual void endUserArea(
        const Location& end_) = 0;

    /**
     * @brief Generate beginning of the suite class
     *
     * @param name_ Name of the suite
     */
    virtual void enterSuite(
        const std::string& name_) = 0;

    /**
     * @brief Generate preamble of the suite's start up function
     */
    virtual void suiteStartUp() = 0;

    /**
     * @brief Generate preamble of the suite's tear down function
     */
    virtual void suiteTearDown() = 0;

    /**
     * @brief Beginning of a test case
     *
     * @param name_ Name of the case
     */
    virtual void enterCase(
        const std::string& name_) = 0;

    /**
     * @brief Generate declaration of the case's start-up function
     */
    virtual void caseStartUp() = 0;

    /**
     * @brief Generate declaration of the case's tear down function
     */
    virtual void caseTearDown() = 0;

    /**
     * @brief Beginning of a test state
     *
     * @param name_ Name of the state
     */
    virtual void enterState(
        const std::string& name_) = 0;

    /**
     * @brief Generate empty function body
     */
    virtual void emptyBody() = 0;

    /**
     * @brief Append a variable without initializer
     *
     * @param name_ Name of the variable
     * @param type_ Name of the type
     */
    virtual void appendVariable(
        const std::string& name_,
        const std::string& type_) = 0;

    /**
     * @brief Append a variable with an initializer
     *
     * @param name_ Name of the variable
     * @param type_ Name of the type
     * @param ibegin_ Beginning of the initializer in the source file
     * @param iend_ End of the initializer in the source file
     */
    virtual void appendVariableInit(
        const std::string& name_,
        const std::string& type_,
        const Location& ibegin_,
        const Location& iend_) = 0;

    /**
     * @brief End of current test state
     */
    virtual void leaveState() = 0;

    /**
     * @brief End of current test case
     */
    virtual void leaveCase() = 0;

    /**
     * @brief Leave current suite
     */
    virtual void leaveSuite() = 0;

    /**
     * @brief Finish the file and copy rest of the source file
     *
     * @param last_ Last copied location in the source file
     */
    virtual void endFile(
        const Location& last_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_GENERATOR_H_ */

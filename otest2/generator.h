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

#include "function.h"
#include "initializer.h"
#include "objecttags.h"

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

    struct AssertionArg {
        Location begin;    /**< beginning location of the argument */
        Location end;      /**< ending location of the argument */
    };

    /**
     * @brief Make a test assertion
     *
     * This method dump arguments of an assertion into the output
     * stream. Just before them it inserts information about file
     * and line number.
     *
     * @param assertion_class_ Name of the assertion class
     * @param assertion_method_ Name of the assertion method
     * @param args_ranges_ Ordered ranges of assertion arguments. The first
     *     range is used as a message in the generic assertion functions.
     */
    virtual void makeAssertion(
        const std::string& assertion_class_,
        const std::string& assertion_method_,
        const std::vector<AssertionArg>& args_ranges_) = 0;

    /**
     * @brief Generate state switch
     *
     * @param state_begin_ Beginning location of the state name
     * @param state_end_ Ending location of the state name
     * @param delay_begin_ Beginning location of the delay expression
     * @param delay_end_ End location of the delay expression
     */
    virtual void makeStateSwitch(
        const Location& state_begin_,
        const Location& state_end_,
        const Location& delay_begin_,
        const Location& delay_end_) = 0;

    /**
     * @brief Begin asserted try/catch block
     *
     * @param begin_ Beginning location of the original try/catch statement
     */
    virtual void makeTryCatchBegin(
        const Location& begin_) = 0;

    /**
     * @brief Generate code for one exception handler
     *
     * @param type_ Type of the handler variable
     * @param varname_ Name of the handler variable
     */
    virtual void makeCatchHandler(
        const std::string& type_,
        const std::string& varname_) = 0;

    /**
     * @brief Finish the asserted try/catch block
     */
    virtual void makeTryCatchEnd() = 0;

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
     * @param tags_ Tags assigned to the suite object
     */
    virtual void enterSuite(
        const std::string& name_,
        const Parser::ObjectTags& tags_) = 0;

    /**
     * @brief Finish block of suite's variables
     */
    virtual void finishSuiteFixtures() = 0;

    /**
     * @brief Finish block of suite's functions
     */
    virtual void finishSuiteFunctions() = 0;

    /**
     * @brief Beginning of a test case
     *
     * @param name_ Name of the case
     * @param tags_ Tags assigned to the suite object
     */
    virtual void enterCase(
        const std::string& name_,
        const Parser::ObjectTags& tags_) = 0;

    /**
     * @brief Finish block of case's variables
     */
    virtual void finishCaseFixtures() = 0;

    /**
     * @brief Finish block of case's functions
     */
    virtual void finishCaseFunctions() = 0;

    /**
     * @brief Beginning of a test state
     *
     * @param name_ Name of the state
     * @param first_ If it's true the state is the entering state - the
     *     scenario
     * @param state_fce_ Description of the state function
     * @param fbegin_ Beginning of the declaration of the state function
     * @param fend_ End of the declaration of the state function
     */
    virtual void enterState(
        const std::string& name_,
        bool first_,
        FunctionPtr state_fce_,
        const Location& fbegin_,
        const Location& fend_) = 0;

    /**
     * @brief Generate empty state
     */
    virtual void emptyState() = 0;

    /**
     * @brief Enter a test section
     *
     * @param name_ Name of the section
     * @param section_begin_ Location of the section body
     */
    virtual void enterSection(
        const std::string& name_,
        const Location& section_begin_) = 0;

    /**
     * @brief Leave an already opened section
     */
    virtual void leaveSection() = 0;

    /**
     * @brief Append a variable
     *
     * @param name_ Name of the variable
     * @param type_ Name of the type
     * @param initializer_ The initializer or null ptr if the initializer
     *     is not defined
     */
    virtual void appendVariable(
        const std::string& name_,
        const std::string& type_,
        InitializerPtr initializer_) = 0;

    /**
     * @brief Append user data variable
     *
     * @param name_ Name of the variable
     * @param key_ Key of the user datum inside the UserData container
     * @param type_ Type of the variable
     */
    virtual void appendUserData(
        const std::string& name_,
        const std::string& key_,
        const std::string& type_) = 0;

    /**
     * @brief Append a start-up function
     *
     * @param function_ The function
     * @param fbegin_ Beginning of function declaration
     * @param fend_ End of function declaration
     */
    virtual void appendStartUpFunction(
        FunctionPtr function_,
        const Location& fbegin_,
        const Location& fend_) = 0;

    /**
     * @brief Append a tear-down function
     *
     * @param function_ The function
     * @param fbegin_ Beginning of function declaration
     * @param fend_ End of function declaration
     */
    virtual void appendTearDownFunction(
        FunctionPtr function_,
        const Location& fbegin_,
        const Location& fend_) = 0;

    /**
     * @brief Append registration of a fixture object
     *
     * @param name_ Name of the fixture
     * @param start_up_ Fixture's start-up method. May be null.
     * @param tear_down_ Fixture's tear-down method. May be null.
     */
    virtual void appendFixtureObject(
        const std::string& name_,
        FunctionPtr start_up_,
        FunctionPtr tear_down_) = 0;

    /**
     * @brief Append a user function
     *
     * @param function_ Description of the function
     * @param fbegin_ Beginning of function declaration
     * @param fend_ End of function declaration
     */
    virtual void appendGenericFunction(
        FunctionPtr function_,
        const Location& fbegin_,
        const Location& fend_) = 0;

    /**
     * @brief Append a repeater
     *
     * @param name_ Name of the repeater variable
     * @param type_ Name of the type
     * @param initializer_ The initializer or null pointer if the initializer
     *     is not defined
     */
    virtual bool appendRepeater(
        const std::string& name_,
        const std::string& type_,
        InitializerPtr initializer_) = 0;

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

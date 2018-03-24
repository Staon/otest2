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
     * @brief Copy part of the source file
     *
     * @param begin_ Beginning location (including)
     * @param end_ End location (including)
     */
    virtual void copySource(
        const Location& begin_,
        const Location& end_) = 0;

    /**
     * @brief Generate beginning of the suite class
     *
     * @param name_ Name of the suite
     */
    virtual void enterSuite(
        const std::string& name_) = 0;

    /**
     * @brief Beginning of a test case
     *
     * @param name_ Name of the case
     */
    virtual void enterCase(
        const std::string& name_) = 0;

    /**
     * @brief Beginning of a test state
     *
     * @param name_ Name of the state
     */
    virtual void enterState(
        const std::string& name_) = 0;

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

//    /**
//     * @brief Append new variable
//     *
//     * @param name_ Name of the variable
//     * @param declaration_ Declaration of the variable
//     */
//    virtual void appendVariable(
//        const dstring& name_,
//        const DeclarationPtr& declaration_) = 0;
//
//    /**
//     * @brief Finish declaration section of current suite/case/state
//     */
//    virtual void finishDeclarations() = 0;
//
//    /**
//     * @brief Set variable initializer
//     *
//     * @param name_ Name of the variable
//     * @param body_ Body of the initializer
//     * @return True if the initializer is set. False if the variable
//     *     doesn't exist.
//     */
//    virtual bool setInitializer(
//        const dstring& name_,
//        const dstring& body_) = 0;
//
//    /**
//     * @brief Set body of the ctor of current object
//     */
//    virtual void setCtorBody(
//        const dstring& body_) = 0;
//
//    /**
//     * @brief Set body of the dtor of current object
//     */
//    virtual void setDtorBody(
//        const dstring& body_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_GENERATOR_H_ */

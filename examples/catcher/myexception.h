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

#ifndef OTest2__EXAMPLES_CATCHER_MYEXCEPTION_H_
#define OTest2__EXAMPLES_CATCHER_MYEXCEPTION_H_

#include <string>

namespace OTest2 {

namespace Examples {

/**
 * @brief Custom base class of exception objects
 */
class MyException {
  public:
    /**
     * @brief Ctor
     */
    MyException();

    /**
     * @brief Move ctor
     */
    MyException(
        MyException&& exc_);

    /**
     * @brief Dtor
     */
    virtual ~MyException();

    /* -- avoid copying */
    MyException(
        const MyException&) = delete;
    MyException& operator = (
        const MyException&) = delete;

    /**
     * @brief Get a message describing the exception
     */
    virtual std::string getMessage() const = 0;
};

/**
 * @brief One simple implementation of the MyException
 */
class MyExceptionMsg : public MyException {
  private:
    std::string message;

  public:
    explicit MyExceptionMsg(
        std::string message_);
    MyExceptionMsg(
        MyExceptionMsg&& exc_);
    virtual ~MyExceptionMsg();

    /* -- avoid copying */
    MyExceptionMsg(
        const MyExceptionMsg&) = delete;
    MyExceptionMsg& operator = (
        const MyExceptionMsg&) = delete;

    virtual std::string getMessage() const;
};

} /* -- namespace Examples */

} /* -- namespace OTest2 */

#endif /* -- OTest2__EXAMPLES_CATCHER_MYEXCEPTION_H_ */

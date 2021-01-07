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

#ifndef OTest2OTEST2_EXCPARSER_H_
#define OTest2OTEST2_EXCPARSER_H_

#include <string>

#include <otest2/exc.h>

namespace OTest2 {

/**
 * @brief An error during parsing of the test file
 */
class ParserException : public Exception {
  private:
    std::string message;   /**< error message */
    std::string file;      /**< location of the error */
    int begin_line;
    int begin_column;
    int end_line;
    int end_column;

  public:
    /**
     * @brief Ctor
     */
    ParserException();

    /**
     * @brief Move ctor
     */
    ParserException(
        ParserException&& exc_) noexcept;

    /**
     * @brief Dtor
     */
    virtual ~ParserException();

    /* -- avoid copying */
    ParserException(
        const ParserException&) = delete;
    ParserException& operator =(
        const ParserException&) = delete;

    /**
     * @brief Set attributes of the exception
     *
     * @param message_ Error message
     * @param file_ location of the error - filename
     * @param begin_line_ location of the error - beginning line number
     * @param begin_column_ location of the error - beginning column number
     * @param end_line_ location of the error - ending line number
     * @param end_column_ location of the error - ending line column
     */
    void setException(
        const std::string& message_,
        const std::string& file_,
        int begin_line_,
        int begin_column_,
        int end_line_,
        int end_column_);

    /* -- exception interface */
    virtual std::string reason() const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_EXCPARSER_H_ */

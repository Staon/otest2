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

#ifndef OTest2__OTEST2_EXCREADER_H_
#define OTest2__OTEST2_EXCREADER_H_

#include <string>

#include "exc.h"

namespace OTest2 {

class ReaderException {
  private:
    std::string message;
    int line;
    int column;

  public:
    /**
     * @brief Ctor
     *
     * @param message_ Error message
     * @param line_ Line number in the source file
     * @param column_ Column number in the source file
     */
    explicit ReaderException(
        const std::string& message_,
        int line_,
        int column_);

    /**
     * @brief Move ctor
     */
    ReaderException(
        ReaderException&& exc_);

    /**
     * @brief Dtor
     */
    virtual ~ReaderException();

    /* -- avoid copying */
    ReaderException(
        const ReaderException&) = delete;
    ReaderException& operator =(
        const ReaderException&) = delete;

    /* -- exception interface */
    virtual std::string reason() const;
};

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_EXCREADER_H_ */

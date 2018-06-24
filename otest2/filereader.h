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

#ifndef OTest2__OTEST2_FILEREADER_H_
#define OTest2__OTEST2_FILEREADER_H_

#include <iosfwd>
#include <string>

#include "location.h"

namespace OTest2 {

/**
 * @brief Reader of an input source file
 *
 * The class reads an input file and gets requested parts of the file.
 * The parts must be ordered and must not overlap!
 */
class FileReader {
  private:
    std::istream *is;
    Location current;
    bool handle_eol;

    bool readNextCharacter(
        char& c_);

  public:
    /* -- avoid copying */
    FileReader(
        const FileReader&) = delete;
    FileReader& operator =(
        const FileReader&) = delete;

    /**
     * @brief Ctor
     *
     * @param is_ An input stream. The ownership is not taken.
     */
    explicit FileReader(
        std::istream* is_);

    /**
     * @brief Dtor
     */
    ~FileReader();

    /**
     * @brief Write specified part into a stream
     *
     * @param os_ The stream
     * @param begin_ Beginning of the part (including)
     * @param end_ End of the part (excluding). Null value means
     *     the end of the file.
     */
    void writePart(
        std::ostream& os_,
        const Location& begin_,
        const Location* end_);

    /**
     * @brief Get specified part
     *
     * @param begin_ Beginning of the part (including)
     * @param end_ End of the part (excluding).
     * @return The text of the part
     */
    std::string getPart(
        const Location& begin_,
        const Location& end_);
};

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_FILEREADER_H_ */

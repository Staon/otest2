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

#ifndef OTest2__INCLUDE_OTEST2_BZIP2ISTREAM_H_
#define OTest2__INCLUDE_OTEST2_BZIP2ISTREAM_H_

#include <istream>

namespace OTest2 {

/**
 * @brief Bzip2 input stream
 */
class Bzip2IStream : public std::istream {
  private:
    class Buffer;
    Buffer* buffer;

  public:
    /**
     * @brief Ctor
     *
     * @param decorated_ A decorated input stream. The ownership is not taken.
     */
    explicit Bzip2IStream(
        std::istream* decorated_);

    /**
     * @brief Dtor
     */
    virtual ~Bzip2IStream();

    /* -- avoid copying */
    Bzip2IStream(
        const Bzip2IStream&) = delete;
    Bzip2IStream& operator = (
        const Bzip2IStream&) = delete;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_BZIP2ISTREAM_H_ */

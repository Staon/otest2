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

#ifndef OTest2__LIB_BASE64ISTREAM_H_
#define OTest2__LIB_BASE64ISTREAM_H_

#include <istream>

namespace OTest2 {

/**
 * @brief Input base64 stream
 */
class Base64IStream : public std::istream {
  private:
    class Buffer;
    Buffer* buffer;

  public:
    /**
     * @brief Ctor
     *
     * @param decorated_ A decorated input stream. The ownership is not taken.
     */
    explicit Base64IStream(
        std::istream* decorated_);

    /**
     * @brief Dtor
     */
    virtual ~Base64IStream();

    /* -- avoid copying */
    Base64IStream(
        const Base64IStream&) = delete;
    Base64IStream& operator = (
        const Base64IStream&) = delete;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__LIB_BASE64ISTREAM_H_ */

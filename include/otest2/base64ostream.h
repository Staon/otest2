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

#ifndef OTest2__LIB_BASE64OSTREAM_H_
#define OTest2__LIB_BASE64OSTREAM_H_

#include <ostream>

namespace OTest2 {

/**
 * @brief Base64 output stream
 */
class Base64OStream : public std::ostream {
  private:
    class Buffer;
    Buffer* buffer;

  public:
    /**
     * @brief Ctor
     *
     * @param decorated_ A decorated output stream. The ownership is not taken.
     * @param line_length_ Length of lines of output data. If the value is
     *     0 the output is formatted into one continuous line. If the value
     *     positive, the output is formatted into lines of this length.
     *     The value must be divisible by 4!
     */
    explicit Base64OStream(
        std::ostream* decorated_,
        int line_length_ = 0);

    /**
     * @brief Dtor
     */
    virtual ~Base64OStream();

    /* -- avoid copying */
    Base64OStream(
        const Base64OStream&) = delete;
    Base64OStream& operator = (
        const Base64OStream&) = delete;

    /**
     * @brief Finish current sequence
     *
     * The method finishes the sequence, inserts appropriate padding
     * and flushes data into the decorated stream.
     *
     * @return False if an error happens
     */
    bool finish() noexcept;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__LIB_BASE64OSTREAM_H_ */

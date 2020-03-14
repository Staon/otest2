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

#ifndef OTest2__LIB_BASE64BUFFER_H_
#define OTest2__LIB_BASE64BUFFER_H_

#include <streambuf>

namespace OTest2 {

/**
 * @brief Base64 stream buffer
 */
class Base64Buffer : public std::streambuf {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     *
     * @param decorated_ The decorated streambuffer. The ownership is not taken,
     *     the object must exist for the whole lifetime.
     * @param width_ If this value is non-zero and positive, the output is
     *     formatted into lines of this width. The value must divisible by 4.
     *     If the value is zero, the output will be one continuous line.
     */
    explicit Base64Buffer(
        std::streambuf* decorated_,
        int width_);

    /**
     * @brief Dtor
     */
    virtual ~Base64Buffer();

    /* -- avoid copying */
    Base64Buffer(
        const Base64Buffer&) = delete;
    Base64Buffer& operator = (
        const Base64Buffer&) = delete;

    /**
     * @brief Close currently opened sequence
     *
     * This method finishes currently opened sequence, flushes it into the
     * decorated stream buffer and adds appropriate base64 padding.
     *
     * @return false if an error happens
     */
    bool close() noexcept;

  private:
    /* -- streambuf interface */
    virtual int overflow(
        int c);

    bool finishOutputData();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__LIB_BASE64BUFFER_H_ */

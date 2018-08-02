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

#ifndef OTest2__OTEST2_LCSTREAM_H_
#define OTest2__OTEST2_LCSTREAM_H_

#include <ostream>
#include <streambuf>

namespace OTest2 {

/**
 * @brief Counter of lines in generated stream
 */
class LCStreamBuffer : public std::streambuf {
  private:
    std::streambuf* decorated;
    int lineno;
    bool crlf;

  public:
    /* -- avoid copying */
    LCStreamBuffer(
        const LCStreamBuffer&) = delete;
    LCStreamBuffer& operator =(
        const LCStreamBuffer&) = delete;

    /**
     * @brief Ctor
     *
     * @param decorated_ Decorated buffer. The ownership is not taken.
     */
    explicit LCStreamBuffer(
        std::streambuf* decorated_);

    /**
     * @brief Dtor
     */
    virtual ~LCStreamBuffer();

    /**
     * @brief Get current line number
     */
    int getLineNo() const;

  private:
    virtual int_type overflow(
        int_type ch_);
    virtual int sync();
};

/**
 * @brief An output stream (a decorator) with counter of lines
 */
class LCStream : public std::ostream {
  private:
    LCStreamBuffer buffer;

  public:
    /* -- avoid copying */
    LCStream(
        const LCStream&) = delete;
    LCStream& operator =(
        const LCStream&) = delete;

    /**
     * @brief Ctor
     *
     * @param decorated_ A decorated stream. The ownership is not taken.
     */
    explicit LCStream(
        std::ostream* decorated_);

    /**
     * @brief Dtor
     */
    virtual ~LCStream();

    /**
     * @brief Get current line number
     */
    int getLineNo() const;
};

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_LCSTREAM_H_ */

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

#ifndef OTest2__INCLUDE_OTEST2_BZIP2OSTREAM_H_
#define OTest2__INCLUDE_OTEST2_BZIP2OSTREAM_H_

#include <ostream>

namespace OTest2 {

/**
 * @brief Bzip2 output stream
 */
class Bzip2OStream : public std::ostream {
  private:
    class Buffer;
    Buffer* buffer;

  public:
    /**
     * @brief Ctor
     *
     * @param decorated_ A decorated output stream (the compressed data
     *     are written into)
     */
    explicit Bzip2OStream(
        std::ostream* decorated_);

    /**
     * @brief Dtor
     */
    virtual ~Bzip2OStream();

    /* -- avoid copying */
    Bzip2OStream(
        const Bzip2OStream&) = delete;
    Bzip2OStream& operator = (
        const Bzip2OStream&) = delete;

    /**
     * @brief Finish the compressed stream
     *
     * The stream finishes the opened compressing sequence. The stream can
     * be re-used after calling of this method.
     */
    void finish();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_BZIP2OSTREAM_H_ */

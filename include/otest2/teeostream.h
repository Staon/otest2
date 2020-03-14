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

#ifndef OTest2__INCLUDE_OTEST2_TEEOSTREAM_H_
#define OTest2__INCLUDE_OTEST2_TEEOSTREAM_H_

#include <initializer_list>
#include <ostream>

namespace OTest2 {

/**
 * @brief An output stream working as the tee unix utility
 */
class TeeOStream : public std::ostream {
  private:
    class Buffer;
    Buffer* buffer;

  public:
    /**
     * @brief Ctor - no sinks
     */
    TeeOStream();

    /**
     * @brief Ctor - list of sinks
     *
     * @param sinks_ The sinks. The ownerships of them is not taken.
     */
    explicit TeeOStream(
        std::initializer_list<std::ostream*> sinks_);

    /**
     * @brief Ctor
     */
    virtual ~TeeOStream();

    /* -- avoid copying */
    TeeOStream(
        const TeeOStream&) = delete;
    TeeOStream& operator = (
        const TeeOStream&) = delete;

    /**
     * @brief Append another sink stream
     *
     * @param sink_ The sink. The ownership is not taken.
     */
    void appendSink(
        std::ostream* sink_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_TEEOSTREAM_H_ */

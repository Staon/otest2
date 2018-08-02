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

#include "lcstream.h"

#include <assert.h>

namespace OTest2 {

LCStreamBuffer::LCStreamBuffer(
    std::streambuf* decorated_) :
  decorated(decorated_),
  lineno(1),
  crlf(false) {
  assert(decorated != nullptr);

  /* -- there is no buffer here, the object only counts lines */
  setp(nullptr, nullptr);
}

LCStreamBuffer::~LCStreamBuffer() {

}

int LCStreamBuffer::getLineNo() const {
  return lineno;
}

LCStreamBuffer::int_type LCStreamBuffer::overflow(
    LCStreamBuffer::int_type ch_) {
  if(!traits_type::eq_int_type(ch_, traits_type::eof())) {
    /* -- handle EOL */
    char_type c_(traits_type::to_char_type(ch_));
    if(c_ == '\r') {
      ++lineno;
      crlf = true;
    } else if(c_ == '\n') {
      if(!crlf)
        ++lineno;
      else
        crlf = false;
    }
    else
      crlf = false;

    /* -- I don't have own buffer and I only redirect characters
     *    to the decorated buffer. */
    return decorated->sputc(traits_type::to_char_type(ch_));
  }
  else
    return traits_type::not_eof(ch_);
}

int LCStreamBuffer::sync() {
  return decorated->pubsync();
}

LCStream::LCStream(
    std::ostream* decorated_) :
  std::ostream(&buffer),
  buffer(decorated_->rdbuf()) {

}

LCStream::~LCStream() {

}

int LCStream::getLineNo() const {
  return buffer.getLineNo();
}

} /* namespace OTest2 */

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

#include <otest2/base64buffer.h>

#include <assert.h>

#include <otest2/utils.h>

namespace OTest2 {

struct Base64Buffer::Impl {
    std::streambuf* decorated;

    int output_raw_width;
    int output_base64_width;
    bool output_newline;
    char* output_buffer;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        std::streambuf* decorated_,
        int width_);
    ~Impl();
};

Base64Buffer::Impl::Impl(
    std::streambuf* decorated_,
    int width_) :
  decorated(decorated_) {
  assert(decorated != nullptr);

  /* -- Create output buffer with size according to requested line width. */
  if(width_ > 0) {
    assert(width_ % 4 == 0);
    output_base64_width = width_;
    output_newline = true;
  }
  else {
    output_base64_width = 80;
    output_newline = false;
  }
  output_raw_width = output_base64_width / 4 * 3;
  output_buffer = new char[output_raw_width];
}

Base64Buffer::Impl::~Impl() {
  delete[] output_buffer;
}

Base64Buffer::Base64Buffer(
    std::streambuf* decorated_,
    int width_) :
  pimpl(new Impl(decorated_, width_)) {

  /* -- set the streambuf pointers */
  setp(pimpl->output_buffer, pimpl->output_buffer + pimpl->output_raw_width);
}

Base64Buffer::~Base64Buffer() {
  odelete(pimpl);
}

bool Base64Buffer::finishOutputData() {
  /* -- compute length of data */
  int length_(pptr() - pimpl->output_buffer);
  assert(length_ >= 0 && length_ <= pimpl->output_raw_width);

  /* -- encode the output buffer */
  static const unsigned char base64_table[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  char buffer_[pimpl->output_base64_width];
  const char* in_(pimpl->output_buffer);
  char* out_(buffer_);
  while(length_ >= 3) {
    *out_++ = base64_table[in_[0] >> 2];
    *out_++ = base64_table[((in_[0] & 0x03) << 4) | (in_[1] >> 4)];
    *out_++ = base64_table[((in_[1] & 0x0f) << 2) | (in_[2] >> 6)];
    *out_++ = base64_table[in_[2] & 0x3f];
    length_ -= 3;
    in_ += 3;
  }
  if(length_ > 0) {
    /* -- Padding should happen only if the sequence is closed. Otherwise,
     *    the buffer divisible by 3. */
    *out_++ = base64_table[in_[0] >> 2];
    if(length_ == 1) {
        *out_++ = base64_table[(in_[0] & 0x03) << 4];
        *out_++ = '=';
    }
    else {
        *out_++ = base64_table[((in_[0] & 0x03) << 4) | (in_[1] >> 4)];
        *out_++ = base64_table[(in_[1] & 0x0f) << 2];
    }
    *out_++ = '=';
  }

  /* -- reset the buffer pointers */
  setp(pimpl->output_buffer, pimpl->output_buffer + pimpl->output_raw_width);

  /* -- push the encoded data into the decorated stream buffer */
  int coded_length_(out_ - buffer_);
  return pimpl->decorated->sputn(buffer_, coded_length_) == coded_length_;
}

int Base64Buffer::overflow(
    int c) {
  /* -- flush current data */
  if(!finishOutputData())
    return traits_type::eof();

  /* -- start new line if it's requested */
  if(pimpl->output_newline)
    pimpl->decorated->sputc('\n');

  /* -- write the character */
  if(c != traits_type::eof()) {
    *pimpl->output_buffer = traits_type::to_char_type(c);
    pbump(1);
  }

  return traits_type::not_eof(c);
}

bool Base64Buffer::close() noexcept {
  return finishOutputData();
}

} /* -- namespace OTest2 */

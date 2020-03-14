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

#include <otest2/base64ostream.h>

#include <assert.h>
#include <streambuf>

#include <otest2/utils.h>

namespace OTest2 {

/**
 * @brief Base64 output stream buffer
 */
class Base64OStream::Buffer : public std::streambuf {
  private:
    std::streambuf* decorated;

    int output_raw_width;
    int output_base64_width;
    bool output_newline;
    char* output_buffer;

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
    explicit Buffer(
        std::streambuf* decorated_,
        int width_);

    /**
     * @brief Dtor
     */
    virtual ~Buffer();

    /* -- avoid copying */
    Buffer(
        const Buffer&) = delete;
    Buffer& operator = (
        const Buffer&) = delete;

    /**
     * @brief Finish currently opened sequence
     *
     * This method finishes currently opened sequence, flushes it into the
     * decorated stream buffer and adds appropriate base64 padding.
     *
     * @return false if an error happens
     */
    bool finish() noexcept;

  private:
    /* -- streambuf interface */
    virtual int overflow(
        int c);

    bool finishData();
};


Base64OStream::Buffer::Buffer(
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

  /* -- set the streambuf pointers */
  setp(output_buffer, output_buffer + output_raw_width);
}

Base64OStream::Buffer::~Buffer() {
  delete[] output_buffer;
}

bool Base64OStream::Buffer::finishData() {
  /* -- compute length of data */
  int length_(pptr() - output_buffer);
  assert(length_ >= 0 && length_ <= output_raw_width);

  /* -- encode the output buffer */
  static const unsigned char base64_table[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  char buffer_[output_base64_width];
  const char* in_(output_buffer);
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
  setp(output_buffer, output_buffer + output_raw_width);

  /* -- push the encoded data into the decorated stream buffer */
  int coded_length_(out_ - buffer_);
  return decorated->sputn(buffer_, coded_length_) == coded_length_;
}

int Base64OStream::Buffer::overflow(
    int c) {
  /* -- flush current data */
  if(!finishData())
    return traits_type::eof();

  /* -- start new line if it's requested */
  if(output_newline)
    decorated->sputc('\n');

  /* -- write the character */
  if(c != traits_type::eof()) {
    *output_buffer = traits_type::to_char_type(c);
    pbump(1);
  }

  return traits_type::not_eof(c);
}

bool Base64OStream::Buffer::finish() noexcept {
  return finishData();
}

Base64OStream::Base64OStream(
    std::ostream* decorated_,
    int line_length_) :
  buffer(new Buffer(decorated_->rdbuf(), line_length_)) {
  rdbuf(buffer);
}

Base64OStream::~Base64OStream() {
  odelete(buffer);
}

bool Base64OStream::finish() noexcept {
  return buffer->finish();
}

} /* -- namespace OTest2 */

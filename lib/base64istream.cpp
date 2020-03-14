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
#include <otest2/base64istream.h>

#include <assert.h>
#include <cctype>
#include <cstdint>
#include <streambuf>

#include <otest2/utils.h>

namespace OTest2 {

class Base64IStream::Buffer : public std::streambuf {
  private:
    std::istream* decorated;

    static const int base64_index[256];

    enum { BUFFER_SIZE = 3 * 26 };
    char buffer[BUFFER_SIZE];
    bool eof;

  public:
    explicit Buffer(
        std::istream* decorated_);
    virtual ~Buffer();

    /* -- avoid copying */
    Buffer(
        const Buffer&) = delete;
    Buffer& operator = (
        const Buffer&) = delete;

  private:
    virtual int underflow() override;

    bool readQuaternion(
        std::uint8_t quaternion_[]);
};

const int Base64IStream::Buffer::base64_index[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
};

Base64IStream::Buffer::Buffer(
    std::istream* decorated_) :
  decorated(decorated_),
  eof(false) {
  assert(decorated != nullptr);

}

Base64IStream::Buffer::~Buffer() {

}

bool Base64IStream::Buffer::readQuaternion(
    std::uint8_t quaternion_[]) {
  int i_(0);
  while(i_ < 4) {
    int ci_(decorated->get());

    /* -- end of file -> no input data or missing padding */
    if(ci_ == traits_type::eof())
      break;

    /* -- skip whitespaces */
    std::uint8_t c_(static_cast<std::uint8_t>(traits_type::to_char_type(ci_)));
    if(std::isspace(c_))
      continue;

    /* -- invalid character */
    if(c_ != '=' && base64_index[c_] == -1)
      break;

    /* -- store the character */
    quaternion_[i_++] = c_;
  }

  return i_ == 4;
}

int Base64IStream::Buffer::underflow() {

  /* -- we have already reached the end of the sequence */
  if(eof) {
    setg(nullptr, nullptr, nullptr);
    return traits_type::eof();
  }

  char* out_(buffer);
  char* const end_(buffer + BUFFER_SIZE);
  while(out_ < end_) {
    /* -- read data from the decorated buffer */
    std::uint8_t quaternion_[4];
    if(!readQuaternion(quaternion_)) {
      /* -- There are no other data or the stream is not correctly padded. */
      eof = true;
      break;
    }

    if(quaternion_[3] != '=') {
      /* -- full quaternion */
      std::uint32_t decoded_(
          base64_index[quaternion_[0]] << 18
          | base64_index[quaternion_[1]] << 12
          | base64_index[quaternion_[2]] << 6
          | base64_index[quaternion_[3]]);
      *out_++ = decoded_ >> 16 & 0xff;
      *out_++ = decoded_ >> 8 & 0xff;
      *out_++ = decoded_ & 0xff;
    }
    else {
      /* -- padding */
      std::uint32_t decoded_(
          base64_index[quaternion_[0]] << 18
          | base64_index[quaternion_[1]] << 12);
      *out_++ = decoded_ >> 16 & 0xff;
      if(quaternion_[2] != '=') {
        decoded_ |= base64_index[quaternion_[2]] << 6;
        *out_++ = decoded_ >> 8 & 0xff;
      }
      eof = true;
      break;
    }
  }

  /* -- Reset the stream buffer pointers. Even if we reached the end of
   *    the sequence, there still can be rest of data. */
  setg(buffer, buffer, out_);

  /* -- return next sequence character */
  if(buffer != out_)
    return traits_type::to_int_type(*buffer);
  else
    return traits_type::eof();
}

Base64IStream::Base64IStream(
    std::istream* decorated_) :
  buffer(new Buffer(decorated_)) {
  rdbuf(buffer);
}

Base64IStream::~Base64IStream() {
  odelete(buffer);
}

} /* -- namespace OTest2 */

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

#include <bzip2ostream.h>

#include <assert.h>
#include <bzlib.h>
#include <cstring>
#include <streambuf>

#include <utils.h>

namespace OTest2 {

class Bzip2OStream::Buffer : public std::streambuf {
  private:
    std::ostream* decorated;

    enum { BUFFER_SIZE = 5000 };
    char buffer[BUFFER_SIZE];
    bz_stream bzip_stream;

  public:
    explicit Buffer(
        std::ostream* decorated_);
    virtual ~Buffer();

    /* -- avoid copying */
    Buffer(
        const Buffer&) = delete;
    Buffer& operator = (
        const Buffer&) = delete;

    void finish(
        bool reuse_);

  private:
    virtual int overflow(
        int c_) override;
};

Bzip2OStream::Buffer::Buffer(
    std::ostream* decorated_) :
  decorated(decorated_) {
  assert(decorated != nullptr);

  /* -- initialize the Bzip2 stream structure */
  std::memset(&bzip_stream, 0, sizeof(bzip_stream));
  auto info_(BZ2_bzCompressInit(&bzip_stream, 1, 0, 0 /* -- default workload */));
  assert(info_ == BZ_OK);

  /* -- initialize the streambuffer pointers */
  setp(buffer, buffer + BUFFER_SIZE);
}

Bzip2OStream::Buffer::~Buffer() {
  finish(false);
}

void Bzip2OStream::Buffer::finish(
    bool reuse_) {
  /* -- get size of already written data */
  unsigned int data_size_(static_cast<unsigned int>(pptr() - buffer));
  assert(data_size_ <= BUFFER_SIZE);
  if(data_size_ == 0) /* -- nothing to do */
    return;

  /* -- prepare input buffer */
  bzip_stream.next_in = buffer;
  bzip_stream.avail_in = data_size_;

  /* -- repeatedly compress remaining data until all is done */
  int info_;
  char obuffer_[BUFFER_SIZE];
  do {
    /* -- compress data */
    bzip_stream.next_out = obuffer_;
    bzip_stream.avail_out = BUFFER_SIZE;
    info_ = BZ2_bzCompress(&bzip_stream, BZ_FINISH);
    assert(info_ == BZ_FINISH_OK || info_ == BZ_STREAM_END);

    /* -- push the compressed data into the decorated stream */
    decorated->write(obuffer_, BUFFER_SIZE - bzip_stream.avail_out);
  }
  while(info_ != BZ_STREAM_END);

  /* -- clean up the old bzip context and create new one */
  info_ = BZ2_bzCompressEnd(&bzip_stream);
  assert(info_ == BZ_OK);
  if(reuse_) {
    info_ = BZ2_bzCompressInit(&bzip_stream, 1, 0, 0 /* -- default workLoad */);
    assert(info_ == BZ_OK);
  }

  /* -- reset the stream buffer pointers */
  setp(buffer, buffer + BUFFER_SIZE);
}

int Bzip2OStream::Buffer::overflow(
    int c_) {
  assert(pptr() - buffer == BUFFER_SIZE);

  /* -- compress data */
  char obuffer_[BUFFER_SIZE];
  bzip_stream.next_in = buffer;
  bzip_stream.avail_in = BUFFER_SIZE;
  bzip_stream.next_out = obuffer_;
  bzip_stream.avail_out = BUFFER_SIZE;
  auto info_(BZ2_bzCompress(&bzip_stream, BZ_RUN));
  assert(info_ == BZ_RUN_OK);
  assert(bzip_stream.avail_in < BUFFER_SIZE); /* -- at least 1 B in the buffer */

  /* -- push compressed data into the decorated stream */
  decorated->write(obuffer_, BUFFER_SIZE - bzip_stream.avail_out);

  /* -- move remaining input data at the beginning of the buffer */
  std::memmove(buffer, bzip_stream.next_in, bzip_stream.avail_in);

  /* -- reset the stream buffer pointers */
  setp(buffer, buffer + BUFFER_SIZE);
  pbump(bzip_stream.avail_in);

  /* -- write next character */
  if(c_ != traits_type::eof()) {
    *pptr() = traits_type::to_char_type(c_);
    pbump(1);
  }

  return traits_type::not_eof(c_);
}

Bzip2OStream::Bzip2OStream(
    std::ostream* decorated_) :
  buffer(new Buffer(decorated_)) {
  rdbuf(buffer);
}

Bzip2OStream::~Bzip2OStream() {
  odelete(buffer);
}

void Bzip2OStream::finish() {
  buffer->finish(true);
}

} /* -- namespace OTest2 */

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

#include <otest2/bzip2istream.h>

#include <assert.h>
#include <bzlib.h>
#include <cstring>
#include <streambuf>

#include <otest2/utils.h>

namespace OTest2 {

class Bzip2IStream::Buffer : public std::streambuf {
  private:
    std::istream* decorated;

    enum { BUFFER_SIZE = 5000 };
    char ibuffer[BUFFER_SIZE];
    char obuffer[BUFFER_SIZE];
    bz_stream bzip_stream;
    bool eof;

  public:
    explicit Buffer(
        std::istream* decorated_);
    virtual ~Buffer();

  private:
    virtual int underflow() override;
};

Bzip2IStream::Buffer::Buffer(
    std::istream* decorated_) :
  decorated(decorated_),
  eof(false) {
  assert(decorated != nullptr);

  /* -- initialize the bzip context */
  std::memset(&bzip_stream, 0, sizeof(bzip_stream));
  auto info_(BZ2_bzDecompressInit(&bzip_stream, 0, 0 /* -- no reducing of memory */));
  assert(info_ == BZ_OK);

  /* -- there are no decompressed data yet, don't set the stream buffer pointers */
}

Bzip2IStream::Buffer::~Buffer() {
  BZ2_bzDecompressEnd(&bzip_stream);
}

int Bzip2IStream::Buffer::underflow() {
  /* -- the end of the stream has been already reached */
  if(eof)
    return traits_type::eof();

  /* -- prepare buffer for decompressed data */
  bzip_stream.next_out = obuffer;
  bzip_stream.avail_out = BUFFER_SIZE;

  int info_;
  do {
    /* -- move remaining input data at the beginning of the input buffer */
    std::memmove(ibuffer, bzip_stream.next_in, bzip_stream.avail_in);

    /* -- read next data from the decorated stream */
    decorated->read(ibuffer, BUFFER_SIZE - bzip_stream.avail_in);
    int read_bytes_(decorated->gcount());
    if(read_bytes_ <= 0 && bzip_stream.avail_in == 0) {
      eof = true;
      return traits_type::eof();
    }

    /* -- decompress data */
    bzip_stream.next_in = ibuffer;
    bzip_stream.avail_in += read_bytes_;
    info_ = BZ2_bzDecompress(&bzip_stream);
    assert(info_ == BZ_OK || info_ == BZ_STREAM_END);
  }
  while(info_ != BZ_STREAM_END && bzip_stream.next_out == obuffer);

  /* -- the stream is finished (the bzlib library detected the end of the
   *    compressed sequence. */
  if(info_ == BZ_STREAM_END) {
    eof = true;

    /* -- no output data are valid only at the end of the sequence */
    if(obuffer == bzip_stream.next_out)
      return traits_type::eof();
  }
  assert(obuffer != bzip_stream.next_out);

  /* -- set the stream buffer pointers */
  setg(obuffer, obuffer, bzip_stream.next_out);

  return traits_type::to_int_type(*obuffer);
}

Bzip2IStream::Bzip2IStream(
    std::istream* decorated_) :
  buffer(new Buffer(decorated_)) {
  rdbuf(buffer);
}

Bzip2IStream::~Bzip2IStream() {
  odelete(buffer);
}

} /* -- namespace OTest2 */

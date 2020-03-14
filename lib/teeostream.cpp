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

#include <otest2/teeostream.h>

#include <assert.h>
#include <streambuf>
#include <vector>

#include <otest2/utils.h>

namespace OTest2 {

class TeeOStream::Buffer : public std::streambuf {
  private:
    std::vector<std::ostream*> sinks;

  public:
    explicit Buffer(
        std::initializer_list<std::ostream*> sinks_);
    virtual ~Buffer();

    /* -- avoid copying */
    Buffer(
        const Buffer&) = delete;
    Buffer& operator = (
        const Buffer&) = delete;

    void appendSink(
        std::ostream* sink_);

  private:
    virtual int overflow(
        int c_) override;
};

TeeOStream::Buffer::Buffer(
    std::initializer_list<std::ostream*> sinks_) :
  sinks(sinks_) {

}

TeeOStream::Buffer::~Buffer() {

}

void TeeOStream::Buffer::appendSink(
    std::ostream* sink_) {
  assert(sink_ != nullptr);
  sinks.push_back(sink_);
}

int TeeOStream::Buffer::overflow(
    int c_) {
  if(c_ != traits_type::eof()) {
    for(std::ostream* sink_ : sinks) {
      sink_->put(traits_type::to_char_type(c_));
    }
  }
  return traits_type::not_eof(c_);
}

TeeOStream::TeeOStream() :
  buffer(new Buffer{}) {
  rdbuf(buffer);
}

TeeOStream::TeeOStream(
    std::initializer_list<std::ostream*> sinks_) :
  buffer(new Buffer(sinks_)) {
  rdbuf(buffer);
}

TeeOStream::~TeeOStream() {
  odelete(buffer);
}

void TeeOStream::appendSink(
    std::ostream* sink_) {
  buffer->appendSink(sink_);
}

} /* -- namespace OTest2 */

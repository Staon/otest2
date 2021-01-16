/*
 * Copyright (C) 2021 Ondrej Starek
 *
 * This file is part of OTest2
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

#include <assertbufferstr.h>

#include <assert.h>

namespace OTest2 {

AssertBufferListener::AssertBufferListener() = default;
AssertBufferListener::~AssertBufferListener() = default;

AssertBufferStr::AssertBufferStr(
    AssertBufferListener* listener_) :
  first_message(true),
  listener(listener_) {
  assert(listener != nullptr);
  setp(tmp_buffer, tmp_buffer + TMP_SIZE);
}

AssertBufferStr::~AssertBufferStr() {
  /* -- ignore unfinished message */
}

void AssertBufferStr::finishTmpBuffer() {
  /* -- write temporary data */
  buffer.sputn(tmp_buffer, pptr() - tmp_buffer);

  /* -- set new buffer */
  setp(tmp_buffer, tmp_buffer + TMP_SIZE);
}

int AssertBufferStr::overflow(
    int c_) {
  finishTmpBuffer();

  /* -- insert the passed character */
  if(c_ != traits_type::eof()) {
    tmp_buffer[0] = traits_type::to_char_type(c_);
    pbump(1);
  }

  return traits_type::not_eof(c_);
}

void AssertBufferStr::setForeground(
    Color color_) {
  /* -- default implementation is to do nothing */
}

void AssertBufferStr::setTextStyle(
    Style style_) {
  /* -- default implementation is to do nothing */
}

void AssertBufferStr::resetAttributes() {
  /* -- default implementation is to do nothing */
}

void AssertBufferStr::commitMessage(
    const Context& context_) {
  /* -- append temporary buffer to the string */
  finishTmpBuffer();

  /* -- get the message */
  std::string message_(buffer.str());
  buffer.str("");

  /* -- emit the event */
  if(first_message) {
    first_message = false;
    listener->commitFirstMessage(context_, message_);
  }
  else
    listener->commitMessage(context_, message_);
}

void AssertBufferStr::commitAssertion(
    const Context& context_) {
  /* -- reset the buffer */
  buffer.str("");
  setp(tmp_buffer, tmp_buffer + TMP_SIZE);
  first_message = true;

  /* -- emit the event */
  listener->commitAssertion(context_);
}

} /* -- namespace OTest2 */

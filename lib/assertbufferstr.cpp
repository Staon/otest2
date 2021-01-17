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
  state(State::IDLE),
  type(Type::ASSERTION),
  listener(listener_) {
  assert(listener != nullptr);
  setp(tmp_buffer, tmp_buffer + TMP_SIZE);
}

AssertBufferStr::~AssertBufferStr() {
  /* -- ignore unfinished message */
}

void AssertBufferStr::openAssertion(
    const AssertBufferAssertData& data_) {
  assert(state == State::IDLE);

  state = State::OPENED;
  type = Type::ASSERTION;
  data = data_;
}

void AssertBufferStr::openError() {
  assert(state == State::IDLE);

  state = State::OPENED;
  type = Type::ERROR;
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
  assert(state != State::IDLE);

  /* -- append temporary buffer to the string */
  finishTmpBuffer();

  /* -- get the message */
  std::string message_(buffer.str());
  buffer.str("");

  /* -- emit the event */
  switch(state) {
    case State::OPENED:
      state = State::ADDITIONAL;
      switch(type) {
        case Type::ASSERTION:
          listener->assertionOpeningMessage(context_, data, message_);
          break;
        case Type::ERROR:
          listener->errorOpeningMessage(context_, message_);
          break;
        default:
          assert(false);
          break;
      }
      break;
    case State::ADDITIONAL:
      switch(type) {
        case Type::ASSERTION:
          listener->assertionAdditionalMessage(context_, data, message_);
          break;
        case Type::ERROR:
          listener->errorAdditionalMessage(context_, message_);
          break;
        default:
          assert(false);
          break;
      }
      break;
    default:
      assert(false);
      break;
  }
}

void AssertBufferStr::commitAssertion(
    const Context& context_) {
  /* -- at least one message must have been constructed */
  assert(state == State::ADDITIONAL);

  /* -- reset the buffer */
  buffer.str("");
  setp(tmp_buffer, tmp_buffer + TMP_SIZE);

  /* -- switch the state */
  state = State::IDLE;

  /* -- emit the event */
  switch(type) {
    case Type::ASSERTION:
      listener->assertionClose(context_, data);
      break;
    case Type::ERROR:
      listener->errorClose(context_);
      break;
    default:
      assert(false);
      break;
  }
}

} /* -- namespace OTest2 */

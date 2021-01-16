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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTBUFFERSTR_H_
#define OTest2_INCLUDE_OTEST2_ASSERTBUFFERSTR_H_

#include <otest2/assertbuffer.h>
#include <sstream>

namespace OTest2 {

class Context;

/**
 * @brief Listener of the string assertion buffer
 */
class AssertBufferListener {
  public:
    /* -- avoid copying */
    AssertBufferListener(
        const AssertBufferListener&) = delete;
    AssertBufferListener& operator = (
        const AssertBufferListener&) = delete;

    /**
     * @brief Ctor
     */
    AssertBufferListener();

    /**
     * @brief Dtor
     */
    virtual ~AssertBufferListener();

    /**
     * @brief First composed assertion message
     */
    virtual void commitFirstMessage(
        const Context& context_,
        const std::string& message_) = 0;

    /**
     * @brief Additional assertion message
     */
    virtual void commitMessage(
        const Context& context_,
        const std::string& message_) = 0;

    /**
     * @brief Closing of the assertion
     */
    virtual void commitAssertion(
        const Context& context_) = 0;
};

/**
 * @brief An implementation of the assertion buffer keeping messages
 *     in a memory buffer
 */
class AssertBufferStr : public AssertBuffer {
  private:
    /* -- static buffer for std::streambuf implementation */
    enum {
      TMP_SIZE = 1024
    };
    char tmp_buffer[TMP_SIZE];

    /* -- dynamic buffer for the whole message */
    std::stringbuf buffer;

    /* -- status flag - first assertion message */
    bool first_message;

    /* -- buffer listener */
    AssertBufferListener* listener;

    void finishTmpBuffer();

  public:
    /* -- avoid copying */
    AssertBufferStr(
        const AssertBufferStr&) = delete;
    AssertBufferStr& operator = (
        const AssertBufferStr&) = delete;

    /**
     * @brief Ctor
     *
     * @param listener_ Listener of the buffer. The ownership is not taken.
     */
    explicit AssertBufferStr(
        AssertBufferListener* listener_);

    /**
     * @brief Dtor
     */
    virtual ~AssertBufferStr();

  protected:
    /* -- stream buffer */
    virtual int overflow(
        int c_) override final;

  public:
    /* -- assert buffer */
    virtual void setForeground(
        Color color_) override;
    virtual void setTextStyle(
        Style style_) override;
    virtual void resetAttributes() override;
    virtual void commitMessage(
        const Context& context_) override final;
    virtual void commitAssertion(
        const Context& context_) override final;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTBUFFERSTR_H_ */

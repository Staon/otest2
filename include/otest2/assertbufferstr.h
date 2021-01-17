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
#include <otest2/assertbufferstrptr.h>
#include <sstream>
#include <string>

namespace OTest2 {

class Context;

/**
 * @brief Common data kept for opened assertion
 */
struct AssertBufferAssertData {
    bool condition;
    std::string file;
    int line;
};

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
     * @brief First composed message of an assertion
     *
     * @param context_ OTest2 context
     * @param data_ Data about the assertion
     * @param message_ The composed message
     */
    virtual void assertionOpeningMessage(
        const Context& context_,
        const AssertBufferAssertData& data_,
        const std::string& message_) = 0;

    /**
     * @brief Additional assertion message
     */
    virtual void assertionAdditionalMessage(
        const Context& context_,
        const AssertBufferAssertData& data_,
        const std::string& message_) = 0;

    /**
     * @brief Closing of the assertion
     */
    virtual void assertionClose(
        const Context& context_,
        const AssertBufferAssertData& data_) = 0;

    /**
     * @brief First composed message of an internal error
     *
     * @param context_ OTest2 context
     * @param message_ The composed message
     */
    virtual void errorOpeningMessage(
        const Context& context_,
        const std::string& message_) = 0;

    /**
     * @brief Additional error message
     */
    virtual void errorAdditionalMessage(
        const Context& context_,
        const std::string& message_) = 0;

    /**
     * @brief Closing of the internal error
     */
    virtual void errorClose(
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

    /* -- object status - opening of the assertion */
    enum class State {
        IDLE = 0,
        OPENED,
        ADDITIONAL,
    } state;
    enum class Type {
        ASSERTION = 0,
        ERROR,
    } type;
    AssertBufferAssertData data;

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

    /**
     * @brief Open an assertion
     *
     * @param data_ Data kept about the assertion
     */
    void openAssertion(
        const AssertBufferAssertData& data_);

    /**
     * @brief Open an internal error
     */
    void openError();

  protected:
    /* -- stream buffer */
    virtual int overflow(
        int c_) override final;

  public:
    /* -- assert buffer */
    virtual void setForeground(
        Color color_) override;
    virtual void setBackground(
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

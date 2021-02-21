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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTBUFFERBASE_H_
#define OTest2_INCLUDE_OTEST2_ASSERTBUFFERBASE_H_

#include <otest2/assertbufferstr.h>

namespace OTest2 {

/**
 * @brief An empty implementation of the assertion buffer
 *
 * This implementation is designated to be a base class for implementing
 * of custom assertion buffers (custom reporters).
 */
class AssertBufferBase : public AssertBufferStr, public AssertBufferListener {
  public:
    /* -- avoid copying */
    AssertBufferBase(
        const AssertBufferBase&) = delete;
    AssertBufferBase& operator = (
        const AssertBufferBase&) = delete;

    /**
     * @brief Ctor
     */
    AssertBufferBase();

    /**
     * @brief Dtor
     */
    virtual ~AssertBufferBase();

    /* -- Empty implementations of assertion functions. User is supposed
     *    to override them. */
    virtual void assertionOpeningMessage(
        const Context& context_,
        const AssertBufferAssertData& data_,
        const std::string& message_) override;
    virtual void assertionAdditionalMessage(
        const Context& context_,
        const AssertBufferAssertData& data_,
        const std::string& message_) override;
    virtual void assertionClose(
        const Context& context_,
        const AssertBufferAssertData& data_) override;
    virtual void errorOpeningMessage(
        const Context& context_,
        const std::string& message_) override;
    virtual void errorAdditionalMessage(
        const Context& context_,
        const std::string& message_) override;
    virtual void errorClose(
        const Context& context_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTBUFFERBASE_H_ */

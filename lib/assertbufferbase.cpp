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

#include <assertbufferbase.h>

namespace OTest2 {

AssertBufferBase::AssertBufferBase() :
  AssertBufferStr(this) {

}

AssertBufferBase::~AssertBufferBase() = default;

void AssertBufferBase::assertionOpeningMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  /* -- empty implementation */
}

void AssertBufferBase::assertionAdditionalMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  /* -- empty implementation */
}

void AssertBufferBase::assertionClose(
    const Context& context_,
    const AssertBufferAssertData& data_) {
  /* -- empty implementation */
}

void AssertBufferBase::errorOpeningMessage(
    const Context& context_,
    const std::string& message_) {
  /* -- empty implementation */
}

void AssertBufferBase::errorAdditionalMessage(
    const Context& context_,
    const std::string& message_) {
  /* -- empty implementation */
}

void AssertBufferBase::errorClose(
    const Context& context_) {
  /* -- empty implementation */
}

} /* -- namespace OTest2 */

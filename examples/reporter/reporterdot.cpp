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

#include "reporterdot.h"

#include <cstdio>

#include <otest2/assertbufferbase.h>
#include <otest2/parameters.h>
#include <otest2/utils.h>

namespace OTest2 {

namespace Examples {

class ReporterDot::AssertBufferDot : public AssertBufferBase {
  public:
    /* -- avoid copying */
    AssertBufferDot(
        const AssertBufferDot&) = delete;
    AssertBufferDot& operator = (
        const AssertBufferDot&) = delete;

    AssertBufferDot() = default;
    virtual ~AssertBufferDot() = default;

    /* -- assert buffer interface */
    virtual void assertionOpeningMessage(
        const Context& context_,
        const AssertBufferAssertData& data_,
        const std::string& message_) override;
    virtual void errorOpeningMessage(
        const Context& context_,
        const std::string& message_) override;
};

void ReporterDot::AssertBufferDot::assertionOpeningMessage(
    const Context& context_,
    const AssertBufferAssertData& data_,
    const std::string& message_) {
  if(!data_.condition)
    std::printf("\n[%s, %d]: %s\n", data_.file.c_str(), data_.line, message_.c_str());
}

void ReporterDot::AssertBufferDot::errorOpeningMessage(
    const Context& context_,
    const std::string& message_) {
  std::printf("\nerror: %s\n", message_.c_str());
}

ReporterDot::ReporterDot() :
  buffer(std::make_shared<AssertBufferDot>()) {

}

ReporterDot::~ReporterDot() {

}

void ReporterDot::enterTest(
    const Context& context_,
    const std::string& name_,
    const Parameters& params_) {

}

void ReporterDot::enterSuite(
    const Context& context_,
    const std::string& name_,
    const Parameters& params_) {

}

void ReporterDot::enterCase(
    const Context& context_,
    const std::string& name_,
    const Parameters& params_) {

}

void ReporterDot::enterState(
    const Context& context_,
    const std::string& name_) {

}

AssertBufferPtr ReporterDot::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& file_,
    int lineno_) {
  buffer->openAssertion({condition_, file_, lineno_});
  return buffer;
}

AssertBufferPtr ReporterDot::enterError(
    const Context& context_) {
  buffer->openError();
  return buffer;
}

void ReporterDot::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {

}

void ReporterDot::leaveCase(
    const Context& context_,
    const std::string& name_,
    const Parameters& params_,
    bool result_) {
  std::printf(".");
  std::fflush(stdout);
}

void ReporterDot::leaveSuite(
    const Context& context_,
    const std::string& name_,
    const Parameters& params_,
    bool result_) {

}

void ReporterDot::leaveTest(
    const Context& context_,
    const std::string& name_,
    const Parameters& params_,
    bool result_) {
  if(result_)
    std::printf("OK\n");
  else
    std::printf("Failed\n");
}

} /* -- namespace Examples */

} /* -- namespace OTest2 */

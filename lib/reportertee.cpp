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

#include <reportertee.h>

#include <algorithm>
#include <assert.h>
#include <functional>
#include <vector>

#include <assertbuffer.h>

namespace OTest2 {

using namespace std::placeholders;

namespace {

class Buffer : public AssertBuffer {
  private:
    std::vector<AssertBufferPtr> buffers;

  public:
    /* -- avoid copying */
    Buffer(
        const Buffer&) = delete;
    Buffer& operator = (
        const Buffer&) = delete;

  public:
    Buffer();
    virtual ~Buffer();

    void appendBuffer(
        AssertBufferPtr buffer_);

  protected:
    /* -- stream buffer */
    virtual int overflow(
        int c_) override final;

  public:
    /* -- assertion buffer */
    virtual void setForeground(
        Color color_) override;
    virtual void setTextStyle(
        Style style_) override;
    virtual void resetAttributes() override;
    virtual void commitMessage(
        const Context& context_) override;
    virtual void commitAssertion(
        const Context& context_) override;
};

Buffer::Buffer() = default;
Buffer::~Buffer() = default;

void Buffer::appendBuffer(
    AssertBufferPtr buffer_) {
  buffers.push_back(buffer_);
}

int Buffer::overflow(
    int c_) {
  if(c_ != traits_type::eof()) {
    std::for_each(
        buffers.begin(),
        buffers.end(),
        std::bind(&AssertBuffer::sputc, _1, traits_type::to_char_type(c_)));
  }
  return traits_type::not_eof(c_);
}

void Buffer::setForeground(
    Color color_) {
  std::for_each(
      buffers.begin(),
      buffers.end(),
      std::bind(&AssertBuffer::setForeground, _1, color_));
}

void Buffer::setTextStyle(
    Style style_) {
  std::for_each(
      buffers.begin(),
      buffers.end(),
      std::bind(&AssertBuffer::setTextStyle, _1, style_));
}

void Buffer::resetAttributes() {
  std::for_each(
      buffers.begin(),
      buffers.end(),
      std::bind(&AssertBuffer::resetAttributes, _1));
}

void Buffer::commitMessage(
    const Context& context_) {
  std::for_each(
      buffers.begin(),
      buffers.end(),
      std::bind(&AssertBuffer::commitMessage, _1, std::cref(context_)));
}

void Buffer::commitAssertion(
    const Context& context_) {
  std::for_each(
      buffers.begin(),
      buffers.end(),
      std::bind(&AssertBuffer::commitAssertion, _1, std::cref(context_)));
}

} /* -- namespace */

ReporterTee::ReporterTee() {

}

ReporterTee::~ReporterTee() {

}

void ReporterTee::appendReporter(
    Reporter* reporter_) {
  assert(reporter_ != nullptr);

  reporters.push_back(reporter_);
}

void ReporterTee::enterTest(
    const Context& context_,
    const std::string& name_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterTest,
          _1,
          std::cref(context_),
          std::cref(name_)));
}

void ReporterTee::enterSuite(
    const Context& context_,
    const std::string& name_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterSuite,
          _1,
          std::cref(context_),
          std::cref(name_)));
}

void ReporterTee::enterCase(
    const Context& context_,
    const std::string& name_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterCase,
          _1,
          std::cref(context_),
          std::cref(name_)));
}

void ReporterTee::enterState(
    const Context& context_,
    const std::string& name_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::enterState,
          _1,
          std::cref(context_),
          std::cref(name_)));
}

AssertBufferPtr ReporterTee::enterAssert(
    const Context& context_,
    bool condition_,
    const std::string& file_,
    int lineno_) {
  auto buffer_(std::make_shared<Buffer>());
  std::for_each(
      reporters.begin(),
      reporters.end(),
      [buffer_, &context_, condition_, &file_, lineno_](Reporter* reporter_) {
        buffer_->appendBuffer(
            reporter_->enterAssert(context_, condition_, file_, lineno_));
        }
  );
  return buffer_;
}

AssertBufferPtr ReporterTee::enterError(
    const Context& context_) {
  auto buffer_(std::make_shared<Buffer>());
  std::for_each(
      reporters.begin(),
      reporters.end(),
      [buffer_, &context_](Reporter* reporter_) {
        buffer_->appendBuffer(
            reporter_->enterError(context_));
      }
  );
  return buffer_;
}

void ReporterTee::leaveState(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveState,
          _1,
          std::cref(context_),
          std::cref(name_),
          result_));
}

void ReporterTee::leaveCase(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveCase,
          _1,
          std::cref(context_),
          std::cref(name_),
          result_));
}

void ReporterTee::leaveSuite(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveSuite,
          _1,
          std::cref(context_),
          std::cref(name_),
          result_));
}

void ReporterTee::leaveTest(
    const Context& context_,
    const std::string& name_,
    bool result_) {
  std::for_each(
      reporters.begin(),
      reporters.end(),
      std::bind(
          &Reporter::leaveTest,
          _1,
          std::cref(context_),
          std::cref(name_),
          result_));
}

} /* -- namespace OTest2 */

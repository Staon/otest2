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

#include <assertstream.h>

#include <assert.h>
#include <iterator>

#include <assertbuffer.h>

namespace OTest2 {

AssertStream::AssertStream(
    const Context& context_,
    AssertBufferPtr buffer_,
    bool result_,
    std::initializer_list<const char*> parameters_) :
  std::ostream(buffer_.get()),
  context(&context_),
  buffer(buffer_),
  result(result_),
  parameters(parameters_) {
  assert(buffer != nullptr);

}

AssertStream::AssertStream(
    AssertStream&& other_) :
  context(other_.context),
  buffer(std::move(other_.buffer)),
  result(other_.result),
  parameters(other_.parameters) {
  other_.buffer = nullptr;
}

AssertStream::~AssertStream() {
  if(buffer != nullptr)
    buffer->commitAssertion(*context);
}

void AssertStream::setForeground(
    Color color_) {
  assert(buffer != nullptr);
  buffer->setForeground(color_);
}

void AssertStream::setBackground(
    Color color_) {
  assert(buffer != nullptr);
  buffer->setBackground(color_);
}

void AssertStream::setTextStyle(
    Style style_) {
  assert(buffer != nullptr);
  buffer->setTextStyle(style_);
}

void AssertStream::resetAttributes() {
  assert(buffer != nullptr);
  buffer->resetAttributes();
}

void AssertStream::printParameter(
    int index_) {
  assert(index_ >= 0 && index_ < parameters.size());
  auto iter_(parameters.begin());
  std::advance(iter_, index_);
  *this << *iter_;
}

void AssertStream::commitMessage() {
  assert(buffer != nullptr);
  buffer->commitMessage(*context);
}

bool AssertStream::getResult() {
  return result;
}

namespace Private {

/**
 * @brief Compile-time sequence of integers
 */
template<int...>
struct NumSequence {

};

/**
 * @brief Compile-time generator of the sequence of integers
 */
template<int N_, int... Seq_>
struct GenSequence : public GenSequence<N_ - 1, N_ - 1, Seq_...> {

};

template<int... Seq_>
struct GenSequence<0, Seq_...> {
  typedef NumSequence<Seq_...> SeqType;
};

template<typename... Values_, int... indexes_>
void invokeFunction(
    NumSequence<indexes_...>,
    AssertStream* stream_,
    void (AssertStream::* fce_)(Values_...),
    const std::tuple<Values_...>& args_) {
  (stream_->*fce_)(std::get<indexes_>(args_)...);
}

template<typename... Values_>
Manipulator<Values_...>::Manipulator(
    void (AssertStream::* fce_)(Values_...),
    Values_... args_) :
  fce(fce_),
  args(args_...) {

}

template<typename... Values_>
std::ostream& Manipulator<Values_...>::doTheJob(
    std::ostream& os_) const {
  AssertStream* stream_(dynamic_cast<AssertStream*>(&os_));
  assert(stream_ != nullptr);
  invokeFunction(
      typename GenSequence<sizeof...(Values_)>::SeqType(), stream_, fce, args);
  return os_;
}

/* -- explicit instantiations */
template class Manipulator<Color>;
template class Manipulator<Style>;
template class Manipulator<int>;
template class Manipulator<>;

} /* -- namespace Private */

template<typename... Values_>
std::ostream& operator << (
    std::ostream& os_,
    const Private::Manipulator<Values_...>& manip_) {
  return manip_.doTheJob(os_);
}

/* -- explicit instantiations */
template std::ostream& operator << (
    std::ostream& os_,
    const Private::Manipulator<Color>&);
template std::ostream& operator << (
    std::ostream& os_,
    const Private::Manipulator<Style>&);
template std::ostream& operator << (
    std::ostream& os_,
    const Private::Manipulator<int>&);
template std::ostream& operator << (
    std::ostream& os_,
    const Private::Manipulator<>&);

Private::Manipulator<Color> foreground(
    Color color_) {
  return Private::Manipulator<Color>(&AssertStream::setForeground, color_);
}

Private::Manipulator<Color> background(
    Color color_) {
  return Private::Manipulator<Color>(&AssertStream::setBackground, color_);
}

Private::Manipulator<Style> textStyle(
    Style style_) {
  return Private::Manipulator<Style>(&AssertStream::setTextStyle, style_);
}

Private::Manipulator<> resetAttrs() {
  return Private::Manipulator<>(&AssertStream::resetAttributes);
}

Private::Manipulator<int> assertPar(
    int index_) {
  return Private::Manipulator<int>(&AssertStream::printParameter, index_);
}

Private::Manipulator<> commitMsg() {
  return Private::Manipulator<>(&AssertStream::commitMessage);
}

} /* -- namespace OTest2 */

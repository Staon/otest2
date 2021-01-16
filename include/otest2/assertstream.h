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

#ifndef OTest2_INCLUDE_OTEST2_ASSERTSTREAM_H_
#define OTest2_INCLUDE_OTEST2_ASSERTSTREAM_H_

#include <assert.h>
#include <initializer_list>
#include <otest2/assertbufferptr.h>
#include <otest2/reporterattributes.h>
#include <ostream>
#include <tuple>

namespace OTest2 {

class Context;

/**
 * @brief Assertion stream
 *
 * The assertion stream is used to construct assertion messages. Beside
 * traditional text formatting it allows to colorize the output.
 */
class AssertStream : public std::ostream {
  private:
    const Context* context;
    AssertBufferPtr buffer;
    bool result;
    std::initializer_list<const char*> parameters;

  public:
    /* -- avoid copying */
    AssertStream(
        const AssertStream&) = delete;
    AssertStream& operator = (
        const AssertStream&) = delete;

    /**
     * @brief Ctor
     *
     * @param context_ OTest2 context
     * @param buffer_ An assertion buffer
     * @param result_ Result of the assertion
     * @param parameters_ Text strings representing assertion parameters.
     */
    explicit AssertStream(
        const Context& context_,
        AssertBufferPtr buffer_,
        bool result_,
        std::initializer_list<const char*> parameters_);

    /**
     * @brief Move ctor
     */
    AssertStream(
        AssertStream&& other_);

    /**
     * @brief Dtor
     */
    virtual ~AssertStream();

    /**
     * @brief Set foreground color
     *
     * @param color_ The color
     */
    void setForeground(
        Color color_);

    /**
     * @brief Set text style
     *
     * @param style_ The style
     */
    void setTextStyle(
        Style style_);

    /**
     * @brief Print assertion parameter at the zero based @a index_
     */
    void printParameter(
        int index_);

    /**
     * @brief Commit the assertion message
     */
    void commitMessage();

    /**
     * @brief Return the assertion result
     *
     * This is a helper method which is designated to be used at the end
     * of the assertion function. The user doesn't keep the result flag
     * himself.
     */
    bool getResult();
};

namespace Private {

template<typename... Values_>
class Manipulator {
  private:
    void (AssertStream::* fce)(Values_...);
    std::tuple<Values_...> args;

  public:
    explicit Manipulator(
        void (AssertStream::* fce_)(Values_...),
        Values_... args_);
    std::ostream& doTheJob(
        std::ostream& os_) const;
};

} /* -- namespace Private */

/**
 * @brief Application of manipulators
 */
template<typename... Values_>
std::ostream& operator << (
    std::ostream& os_,
    const Private::Manipulator<Values_...>& manip_);

/**
 * @brief Foreground color manipulator
 *
 * @param color_ The color
 */
Private::Manipulator<Color> foreground(
    Color color_);

/**
 * @brief Text style manipulator
 *
 * @param style_ The text style
 */
Private::Manipulator<Style> textStyle(
    Style style_);

/**
 * @brief Print assertion parameter
 *
 * @param index_ Zero based index of the parameter
 */
Private::Manipulator<int> assertPar(
    int index_);

/**
 * @brief Commit current assertion message
 */
Private::Manipulator<> commitMsg();

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_ASSERTSTREAM_H_ */

/*
 * Copyright (C) 2019 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_EXCCATCHER_H_
#define OTest2__INCLUDE_OTEST2_EXCCATCHER_H_

#include <functional>
#include <string>

namespace OTest2 {

class Context;

/**
 * @brief Generic exception catcher interface
 *
 * This interface allows to catch exceptions raised inside a code functional.
 * It's used to catch and report unexpected exceptions inside user code
 * or inside exception assertions. Real catcher implementation is set in
 * the OTest2 context. User of the framework can make own implementation
 * to adjust the framework for its environment.
 */
class ExcCatcher {
  public:
    /**
     * @brief Ctor
     */
    ExcCatcher();

    /**
     * @brief Dtor
     */
    virtual ~ExcCatcher();

    /* -- avoid copying */
    ExcCatcher(
        const ExcCatcher&) = delete;
    ExcCatcher& operator =(
        const ExcCatcher&) = delete;

    /**
     * @brief Catch unexpected exceptions
     *
     * @param[in] context_ The OTest2 context
     * @param[in] ftor_ the functional which the exceptions can occurs in.
     * @param[out] message_ If an exception happens an error message is
     *     filled in.
     * @return True if an exception happens. False otherwise.
     */
    virtual bool catchException(
        const Context& context_,
        std::function<void(const Context&)> ftor_,
        std::string& message_) noexcept = 0;
};

} /* namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_EXCCATCHER_H_ */

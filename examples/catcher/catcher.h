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

#ifndef OTest2__EXAMPLES_CATCHER_CATCHER_H_
#define OTest2__EXAMPLES_CATCHER_CATCHER_H_

#include <otest2/exccatcher.h>

namespace OTest2 {

namespace Examples {

/**
 * @brief Example of custom catcher of unhandled exceptions
 */
class Catcher : public ExcCatcher {
  public:
    /**
     * @brief Ctor
     */
    Catcher();

    /**
     * @brief Dtor
     */
    virtual ~Catcher();

    /* -- avoid copying */
    Catcher(
        const Catcher&) = delete;
    Catcher& operator = (
        const Catcher&) = delete;

    /* -- catcher interface */
    virtual bool catchException(
        const Context& context_,
        std::function<void(const Context&)> ftor_,
        std::string& message_) noexcept override;
};

} /* -- namespace Examples */

} /* -- namespace OTest2 */

#endif /* -- OTest2__EXAMPLES_CATCHER_CATCHER_H_ */

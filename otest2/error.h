/*
 * Copyright (C) 2018 Ondrej Starek
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

#ifndef OTest2__OTEST2_ERROR_H_
#define OTest2__OTEST2_ERROR_H_

#include <string>

#include <otest2/exc.h>

namespace OTest2 {

/**
 * @brief Generic error exception
 */
class Error : public Exception {
  private:
    std::string message;

  public:
    /**
     * @brief Ctor
     *
     * @param message_ An error message
     */
    explicit Error(
        const std::string& message_);

    /**
     * @brief Move ctor
     */
    Error(
        Error&& exc_) noexcept;

    /**
     * @brief Dtor
     */
    virtual ~Error();

    /* -- avoid copying */
    Error(
        const Error&) = delete;
    Error& operator =(
        const Error&) = delete;

    /* -- exception interface */
    virtual std::string reason() const;
};

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_ERROR_H_ */

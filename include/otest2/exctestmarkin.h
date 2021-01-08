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

#ifndef OTest2__INCLUDE_OTEST2_EXCTESTMARKIN_H_
#define OTest2__INCLUDE_OTEST2_EXCTESTMARKIN_H_

#include <exception>
#include <string>

#include <otest2/exc.h>

namespace OTest2 {

/**
 * @brief An exception thrown from the testmark deserializer
 */
class ExcTestMarkIn : public Exception {
  private:
    std::string message;

  public:
    /**
     * @brief Ctor
     *
     * @param message_ An error message
     */
    explicit ExcTestMarkIn(
        const std::string& message_);

    /**
     * @brief Move ctor
     */
    ExcTestMarkIn(
        ExcTestMarkIn&& exc_) noexcept;

    /**
     * @brief Dtor
     */
    virtual ~ExcTestMarkIn();

    /* -- avoid copying */
    ExcTestMarkIn(
        const ExcTestMarkIn&) = delete;
    ExcTestMarkIn& operator = (
        const ExcTestMarkIn&) = delete;

    /* -- exception interface */
    virtual std::string reason() const override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_EXCTESTMARKIN_H_ */

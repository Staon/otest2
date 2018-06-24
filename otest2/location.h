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

#ifndef OTest2__OTEST2_LOCATION_H_
#define OTest2__OTEST2_LOCATION_H_

#include <iosfwd>

namespace OTest2 {

/**
 * @brief Location in the source file
 */
class Location {
  private:
    int line;
    int column;

  public:
    /**
     * @brief Ctor - (1,1) location
     */
    Location();

    /**
     * @brief Ctor
     *
     * @param line_ Initial line number
     * @param column_ Initial column number
     */
    explicit Location(
        int line_,
        int column_);

    /**
     * @brief Copy ctor
     */
    Location(
        const Location& loc_);

    /**
     * @brief Dtor
     */
    ~Location();

    /**
     * @brief Swap contents
     */
    void swap(
        Location& loc_) noexcept;

    /**
     * @brief Assignment operator
     */
    Location& operator = (
        const Location& loc_);

    /**
     * @brief Get line
     */
    int getLine() const;

    /**
     * @brief Get column
     */
    int getColumn() const;

    /**
     * @brief Compare 2 locations
     *
     * @param l2_ Second location
     * @return -1 for <, 0 for ==, 1 for >
     */
    int compare(
        const Location& l2_) const;

    /**
     * @brief Move location to next character
     */
    void incChar();

    /**
     * @brief Move location to next line
     */
    void nextLine();

    /**
     * @brief Print the location
     *
     * @param os_ An output stream
     * @return The output stream
     */
    std::ostream& printObject(
        std::ostream& os_) const;
};

std::ostream& operator << (
    std::ostream& os_,
    const Location& loc_);

bool operator < (
    const Location& l1_,
    const Location& l2_);
bool operator <= (
    const Location& l1_,
    const Location& l2_);
bool operator > (
    const Location& l1_,
    const Location& l2_);
bool operator >= (
    const Location& l1_,
    const Location& l2_);
bool operator == (
    const Location& l1_,
    const Location& l2_);
bool operator != (
    const Location& l1_,
    const Location& l2_);

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_LOCATION_H_ */

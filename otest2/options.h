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

#ifndef OTest2__OTEST2_OPTIONS_H_
#define OTest2__OTEST2_OPTIONS_H_

#include <string>
#include <vector>

namespace OTest2 {

/**
 * @brief Options of the otest2 parser
 */
struct Options {
  public:
    std::string infile;
    std::string outfile;
    std::vector<std::string> includes;

    /* -- avoid copying */
    Options(
        const Options&) = delete;
    Options& operator =(
        const Options&) = delete;

    /**
     * @brief Ctor
     */
    Options();

    /**
     * @brief Dtor
     */
    ~Options();

    /**
     * @brief This method validates the options
     *
     * The method validates and computes default option values
     *
     * @return True if the values are OK
     */
    bool validateOptions();
};

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_OPTIONS_H_ */

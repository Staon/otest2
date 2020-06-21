/*
 * Copyright (C) 2020 Ondrej Starek
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

#ifndef OTest2_OTEST2_INITIALIZER_H_
#define OTest2_OTEST2_INITIALIZER_H_

#include <iosfwd>
#include <memory>
#include <string>

#include "location.h"

namespace OTest2 {

class FileReader;

/**
 * @brief Type of the initializer
 */
enum class InitializerType {
  CALL_INIT = 0,  /**< initializer by round brackets */
  LIST_INIT,      /**< initializer by curly brackets (C++11) */
  LIST_INIT_ARG,  /**< initializer by curly brackets matched with std::initializer_list
                       argument */
};

/**
 * @brief Generic initializer interface
 */
class Initializer {
  private:
    InitializerType type;
    Location begin;
    Location end;
    std::string initializer_text;

  public:
    /* -- avoid copying */
    Initializer(
        const Initializer&) = delete;
    Initializer& operator = (
        const Initializer&) = delete;

    /**
     * @brief Ctor
     *
     * @param type_ Type of the initializer
     * @param begin_ Begin location
     * @param end_ End location
     */
    explicit Initializer(
        InitializerType type_,
        const Location& begin_,
        const Location& end_);

    /**
     * @brief Dtor
     */
    ~Initializer();

    /**
     * @brief Read the initializer text from the source file
     *
     * @param reader_ The file reader
     */
    void materializeInitializer(
        FileReader& reader_);

    /**
     * @brief Print variable initialization
     *
     * @param os_ An output stream
     * @param name_ Name of the initialized variable
     */
    void printVarInitialization(
        std::ostream& os_,
        const std::string& name_) const;

    /**
     * @brief Print argument of the repeater factory function
     *
     * @param os_ An output stream
     * @param name_ Name of the initialized variable
     */
    void printArgInitialization(
        std::ostream& os_,
        const std::string& name_) const;
};

typedef std::shared_ptr<Initializer> InitializerPtr;

} /* -- namespace OTest2 */

#endif /* -- OTest2_OTEST2_INITIALIZER_H_ */

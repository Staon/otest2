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
  private:
    std::string config_file;
    std::string domain;
    std::string infile;
    std::string outfile;
    std::vector<std::string> compiler_options;

  public:
    /**
     * @brief Ctor
     *
     * @param argc_ Number of command line arguments
     * @param argv_ The command line arguments
     */
    explicit Options(
        int argc_,
        char* argv_[]);

    /**
     * @brief Dtor
     */
    ~Options();

    /* -- avoid copying */
    Options(
        const Options&) = delete;
    Options& operator =(
        const Options&) = delete;

    /**
     * @brief Get input file name
     */
    const std::string& getInfile() const noexcept;

    /**
     * @brief Get output file name
     */
    const std::string& getOutfile() const noexcept;

    /**
     * @brief Get the OTest2 registration domain
     */
    const std::string& getDomain() const noexcept;

    /**
     * @brief Fill options for the Clang tool
     *
     * @param[out] options_ The options
     */
    void fillClangToolOptions(
        std::vector<std::string>& options_) const;
};

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_OPTIONS_H_ */

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

#include "options.h"

#include <regex>

namespace OTest2 {

Options::Options() {

}

Options::~Options() {

}

bool Options::validateOptions() {
  /* -- the infile is mandatory */
  if(infile.empty())
    return false;

  /* -- compute output file, if it's not specified */
  if(outfile.empty()) {
    outfile = std::regex_replace(infile, std::regex("[.]ot2$"), "") + ".cpp";
  }

  return true;
}

} /* namespace OTest2 */

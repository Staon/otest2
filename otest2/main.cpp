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

#include <cstddef>
#include <exception>
#include <getopt.h>
#include <iostream>
#include <string>

#include "options.h"
#include <otest2/exc.h>
#include "parser.h"

int main(
    int argc_,
    char* argv_[]) {
  try {
    ::OTest2::Options options_(argc_, argv_);
    ::OTest2::parse(options_);
  }
  catch(::OTest2::Exception& exc_) {
    std::cerr << exc_.reason() << std::endl;
    return -1;
  }
  catch(std::exception& exc_) {
    std::cerr << exc_.what() << std::endl;
    return -1;
  }
  catch(...) {
    std::cerr << "Unknown error" << std::endl;
    return -1;
  }

  return 0;
}

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

#include <exception>
#include <getopt.h>
#include <iostream>
#include <cstddef>
#include <string>

#include "exc.h"
#include "options.h"
#include "parser.h"

namespace {

void printHelp(
    std::ostream& os_) {
  os_ << "otest2 - Copyright (C) 2018 Ondrej Starek" << std::endl;
  os_ << std::endl;
  os_ << "  OTest2 preprocessor" << std::endl;
  os_ << std::endl;
  os_ << "Usage: otest2 [-o outfile] [-d domain] [-I path] infile" << std::endl;
  os_ << std::endl;
  os_ << "  -o outfile  --outfile=outfile  Name of the output file." << std::endl;
  os_ << "                                 By default, the name is derived" << std::endl;
  os_ << "                                 from the input filename." << std::endl;
  os_ << "  -d domain   --domain=domain    Name of the test domain." << std::endl;
  os_ << "                                 The default value is 'default'." << std::endl;
  os_ << "  -I path                        An include path." << std::endl;
  os_ << std::endl;
}

void parseCommandLine(
    int argc_,
    char* argv_[],
    ::OTest2::Options& options_) {

  int opt_;
  option longopts_[] = {
      {"outfile", required_argument, nullptr, 'o'},
      {"domain", required_argument, nullptr, 'd'},
      {nullptr, 0, nullptr, 0}
  };
  while((opt_ = getopt_long(argc_, argv_, "o:d:I:", longopts_, nullptr)) != EOF) {
    switch(opt_) {
      case 'o':
        options_.outfile = optarg;
        break;
      case 'd':
        options_.domain = optarg;
        break;
      case 'I':
        options_.includes.push_back(optarg);
        break;
      default:
        printHelp(std::cerr);
        std::exit(-1);
    }
  }

  if(optind < argc_) {
    options_.infile = argv_[optind];
  }

  if(!options_.validateOptions()) {
    printHelp(std::cerr);
    std::exit(-1);
  }
}

} /* -- namespace */

int main(
    int argc_,
    char* argv_[]) {
  try {
    ::OTest2::Options options_;
    parseCommandLine(argc_, argv_, options_);
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

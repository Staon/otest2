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

#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>

#include "error.h"

namespace OTest2 {

namespace {

const char LOCAL_CONFIG_FILE_NAME[] = ".otest2_includes";
const char GLOBAL_CONFIG_FILE[] = "/etc/otest2_includes.conf";

void printHelp(
    std::ostream& os_) {
  os_ << "otest2 - Copyright (C) 2018-2020 Ondrej Starek" << std::endl;
  os_ << std::endl;
  os_ << "  OTest2 preprocessor" << std::endl;
  os_ << std::endl;
  os_ << "Usage: otest2 -s srcfile [-o outfile] [-d domain] [-- compiler options...]" << std::endl;
  os_ << std::endl;
  os_ << "  -h          --help             Print this help message." << std::endl;
  os_ << "  -s infile   --srcfile=infile   Name of the input file." << std::endl;
  os_ << "  -o outfile  --outfile=outfile  Name of the output file. By default, the name" << std::endl;
  os_ << "                                 is derived from the input filename by adding" << std::endl;
  os_ << "                                 .cpp suffix." << std::endl;
  os_ << "  -d domain   --domain=domain    Name of the test domain. The default value is" << std::endl;
  os_ << "                                 'default'." << std::endl;
  os_ << std::endl;
  os_ << "  compiler options...            Compiler flags passed into the clang tool." << std::endl;
  os_ << "                                 The main usage is passing additive include" << std::endl;
  os_ << "                                 directories -Idir. Don't forget to precede" << std::endl;
  os_ << "                                 -- to the compiler options." << std::endl;
  os_ << std::endl;
}

void trimString(
    std::string& str_)
{
  str_.erase(str_.find_last_not_of("\t\n\v\f\r ") + 1);
  str_.erase(0, str_.find_first_not_of("\t\n\v\f\r "));
}

void fillConfiguredIncludeDirectories(
    std::vector<std::string>& options_,
    const std::string& cfg_file_) {
  std::ifstream ifs_(cfg_file_);
  while(!!ifs_) {
    std::string line_;
    std::getline(ifs_, line_);
    trimString(line_);
    if(!line_.empty())
      options_.push_back("--extra-arg-before=-I" + line_);
  }
}

} /* -- namespace */

Options::Options(
    int argc_,
    char* argv_[]) :
  domain("default") {

  /* -- parse command line options */
  struct option longopts_[] = {
      {"help", no_argument, nullptr, 'h'},
      {"domain", required_argument, nullptr, 'd'},
      {"srcfile", required_argument, nullptr, 's'},
      {"outfile", required_argument, nullptr, 'o'},
      {nullptr, 0, nullptr, 0}
  };
  int opt_;
  while((opt_ = getopt_long(argc_, argv_, "hd:s:o:", longopts_, nullptr)) != EOF) {
    switch(opt_) {
      case 'h':
        printHelp(std::cout);
        std::exit(0);
      case 'd':
        domain = optarg;
        break;
      case 's':
        infile = optarg;
        break;
      case 'o':
        outfile = optarg;
        break;
      default:
        printHelp(std::cerr);
        std::exit(-1);
    }
  }

  /* -- check name of the source file */
  if(infile.empty())
    throw Error("no input file is specified");

  /* -- compute name of the output file, if it's not specified */
  if(outfile.empty()) {
    outfile = infile + ".cpp";
  }

  /* -- store options of the compiler */
  for(int i_(optind); i_ < argc_; ++i_)
    compiler_options.emplace_back(argv_[i_]);
}

Options::~Options() {

}

const std::string& Options::getInfile() const noexcept {
  return infile;
}

const std::string& Options::getOutfile() const noexcept {
  return outfile;
}

const std::string& Options::getDomain() const noexcept {
  return domain;
}

void Options::fillClangToolOptions(
    std::vector<std::string>& options_) const {
  /* -- name of the application shown in the output messages */
  options_.emplace_back("otest2");

  /* -- the OTest2 DSL is a C++ code */
  options_.emplace_back("--extra-arg-before=-xc++");

  /* -- the OTest2 DSL needs at least C++11 */
  options_.emplace_back("--extra-arg-before=--std=c++11");

  /* -- turn on conditional compilation */
  options_.emplace_back("--extra-arg-before=-DOTEST2_PARSER_ACTIVE");

  /* -- push compiler options passed by user from the command line */
  for(const auto& opt_ : compiler_options)
    options_.emplace_back("--extra-arg-before=" + opt_);

  /* -- Insert configured include directories. The configuration is read
   *    from several configuration files:
   *      1) .otest2_includes in current directory
   *      2) ~/.otest2_includes in user's home directory
   *      3) ${CMAKE_INSTALL_PREFIX}/lib/otest2_includes
   */
  fillConfiguredIncludeDirectories(options_, LOCAL_CONFIG_FILE_NAME);
  const char* home_dir_(std::getenv("HOME"));
  if(home_dir_ != nullptr) {
    std::string home_cfg_(home_dir_);
    home_cfg_ += "/";
    home_cfg_ += LOCAL_CONFIG_FILE_NAME;
    fillConfiguredIncludeDirectories(options_, home_cfg_);
  }
  fillConfiguredIncludeDirectories(options_, GLOBAL_CONFIG_FILE);

  /* -- add the input file */
  options_.push_back(infile);

  /* -- this avoid searching for the Clang's compilation database */
  options_.emplace_back("--");
}

} /* namespace OTest2 */

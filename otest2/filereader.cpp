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

#include <assert.h>
#include <istream>
#include <ostream>
#include <sstream>

#include <iostream>

#include "excreader.h"
#include "filereader.h"

namespace OTest2 {

FileReader::FileReader(
    std::istream* is_) :
  is(is_),
  current(),
  handle_eol(false) {
  assert(is != nullptr);

}

FileReader::~FileReader() {

}

bool FileReader::readNextCharacter(
    char& c_) {
  int ch_(is->get());

  /* -- end of file or an error */
  if(ch_ == std::istream::traits_type::eof()) {
    if(is->eof()) {
      c_ = 0;
      return true;
    }
    else
      return false;
  }

  /* -- handle end of line */
  if(ch_ == '\r') {
    current.nextLine();
    handle_eol = true;  /* -- Windows CRLF */
  }
  else if(ch_ == '\n') {
    if(handle_eol)  /* -- ignore Windows CRLF */
      handle_eol = false;
    else
      current.nextLine();
  }
  else
    current.incChar();

  c_ = static_cast<char>(ch_);
  return true;
}

void FileReader::writePart(
    std::ostream& os_,
    const Location& begin_,
    const Location* end_) {
  assert(current <= begin_ && (end_ == nullptr || begin_ <= *end_));

  /* -- skip text before the part */
  char c_;
  while(current < begin_) {
    if(!readNextCharacter(c_) || c_ == 0)
      throw ReaderException("IO error", current.getLine(), current.getColumn());
  }

  /* -- copy the part */
  if(end_ != nullptr) {
    while(current < *end_) {
      if(!readNextCharacter(c_) || c_ == 0)
        throw ReaderException("IO error", current.getLine(), current.getColumn());
      os_.put(c_);
    }
  }
  else {
    while(true) {
      if(!readNextCharacter(c_))
        throw ReaderException("IO error", current.getLine(), current.getColumn());
      if(c_ == 0)
        break;
      os_.put(c_);
    }
  }
}

std::string FileReader::getPart(
    const Location& begin_,
    const Location& end_) {
  std::stringstream sos_;
  writePart(sos_, begin_, &end_);
  return sos_.str();
}

} /* namespace OTest2 */

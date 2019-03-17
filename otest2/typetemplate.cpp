/*
 * Copyright (C) 2019 Ondrej Starek
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

#include "typetemplate.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace OTest2 {

namespace {

class Automaton {
  private:
    enum State {
      ORDINARY,
      ARG,
      BARE_NUM,
      PAR_NUM,
      PAR_NUM2,
    };
    State state;
    int argnum;

  public:
    /* -- avoid copying */
    Automaton(
        const Automaton&) = delete;
    Automaton& operator =(
        const Automaton&) = delete;

    Automaton();
    ~Automaton();
    void expandArg(
        std::ostream& os_,
        const std::vector<std::string>& args_);
    bool step(
        std::ostream& os_,
        int c_,
        const std::vector<std::string>& args_);
};

Automaton::Automaton() :
  state(ORDINARY),
  argnum(0) {

}

Automaton::~Automaton() {

}

void Automaton::expandArg(
    std::ostream& os_,
    const std::vector<std::string>& args_) {
  if(argnum > 0 && argnum <= static_cast<int>(args_.size())) {
    os_ << args_[argnum - 1];
  }
  else {
    os_ << "??";
  }
}

bool Automaton::step(
    std::ostream& os_,
    int c_,
    const std::vector<std::string>& args_) {
  switch(state) {
    case ORDINARY:
      if(c_ == -1) {
        return true;
      }
      else if(c_ == '$') {
        state = ARG;
        argnum = 0;
        return true;
      }
      else {
        os_.put(c_);
        return true;
      }

    case ARG:
      if(c_ == -1) {
        return false;
      }
      else if(c_ == '{') {
        state = PAR_NUM;
        return true;
      }
      else if(c_ >= '0' && c_ <= '9') {
        state = BARE_NUM;
        argnum = c_ - '0';
        return true;
      }
      else {
        return false;
      }

    case BARE_NUM:
      if(c_ == -1) {
        expandArg(os_, args_);
        return true;
      }
      else if(c_ >= '0' && c_ <= '9') {
        argnum = argnum * 10 + (c_ - '0');
        return true;
      }
      else if(c_ == '$') {
        expandArg(os_, args_);
        state = ARG;
        return true;
      }
      else {
        expandArg(os_, args_);
        state = ORDINARY;
        os_.put(c_);
        return true;
      }

    case PAR_NUM:
      if(c_ >= '0' && c_ <= '9') {
        state = PAR_NUM2;
        argnum = (c_ - '0');
      }
      else {
        return false;
      }

    case PAR_NUM2:
      if(c_ == '}') {
        expandArg(os_, args_);
        state = ORDINARY;
        return true;
      }
      else if(c_ >= '0' && c_ <= '9') {
        argnum = argnum * 10 + (c_ - '0');
        return true;
      }
      else {
        return false;
      }
  }

  return false;
}

} /* -- namespace */

bool expandTemplate(
    std::ostream& os_,
    const std::string& template_,
    const std::vector<std::string>& args_) {
  Automaton parser_;
  for(auto c_ : template_) {
    if(!parser_.step(os_, c_, args_))
      return false;
  }
  return parser_.step(os_, -1, args_);
}

bool expandTemplates(
    std::vector<std::string>& templates_,
    const std::vector<std::string>& args_) {
  for(auto& template_ : templates_) {
    std::ostringstream sos_;
    if(!expandTemplate(sos_, template_, args_))
      return false;
    template_ = sos_.str();
  }
  return true;
}

} /* namespace OTest2 */

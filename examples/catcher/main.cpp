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

#include <otest2/dfltenvironment.h>
#include <otest2/dfltloop.h>

#include "catcher.h"

int main(
    int argc_,
    char* argv_[]) {
  /* -- prepare the testing environment */
  ::OTest2::Examples::Catcher exc_catcher_;
  ::OTest2::DfltEnvironment environment_(argc_, argv_);
  environment_.setExceptionCatcher(&exc_catcher_);

  /* -- run the test */
  return ::OTest2::defaultMainLoop(environment_.getRunner());
}

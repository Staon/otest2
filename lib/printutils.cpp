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
#include <printutils.h>

#include <iostream>

namespace OTest2 {

void printOrdinalNumber(
    std::ostream& os_,
    int number_) {
  /* -- the number is always the same */
  os_ << number_;

  /* -- resolve the suffix */
  if(number_ > 10 && number_ < 20) {
    os_ << "th";
  }
  else {
    if(number_ < 0)
      number_ *= -1;
    switch(number_ % 10) {
      case 1:
        os_ << "st";
        break;
      case 2:
        os_ << "nd";
        break;
      case 3:
        os_ << "rd";
        break;
      default:
        os_ << "th";
        break;
    }
  }
}

} /* -- namespace OTest2 */

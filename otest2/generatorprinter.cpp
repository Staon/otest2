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

#include "generatorprinter.h"

#include <iostream>

#include "location.h"

namespace OTest2 {

GeneratorPrinter::GeneratorPrinter() {

}

GeneratorPrinter::~GeneratorPrinter() {

}

void GeneratorPrinter::beginFile() {
  std::cout << "begin file" << std::endl;
}

void GeneratorPrinter::copySource(
    const Location& begin_,
    const Location& end_) {
  std::cout << "copy source: " << begin_ << '-' << end_ << std::endl;
}

void GeneratorPrinter::enterSuite(
    const std::string& name_) {
  std::cout << "enter suite: " << name_ << std::endl;
}

void GeneratorPrinter::enterCase(
    const std::string& name_) {
  std::cout << "enter case: " << name_ << std::endl;
}

void GeneratorPrinter::enterState(
    const std::string& name_) {
  std::cout << "enter state: " << name_ << std::endl;
}

void GeneratorPrinter::leaveState() {
  std::cout << "leave state" << std::endl;
}

void GeneratorPrinter::leaveCase() {
  std::cout << "leave case" << std::endl;
}

void GeneratorPrinter::leaveSuite() {
  std::cout << "leave suite" << std::endl;
}

} /* namespace OTest2 */

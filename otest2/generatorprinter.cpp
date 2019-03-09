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

void GeneratorPrinter::startUserArea(
    const Location& begin_) {
  std::cout << "begin user area: " << begin_ << std::endl;
}

void GeneratorPrinter::copySource(
    const Location& begin_,
    const Location& end_) {
  std::cout << "copy source: " << begin_ << "-" << end_ << std::endl;
}

void GeneratorPrinter::makeAssertion(
    const Location& begin_,
    const Location& end_,
    bool insert_expr_,
    const Location& expr_end_) {
  std::cout << "make assertion: " << begin_ << " " << end_
      << " " << insert_expr_ << " " << expr_end_ << std::endl;
}

void GeneratorPrinter::endUserArea(
    const Location& end_) {
  std::cout << "end user area: " << end_ << std::endl;
}

void GeneratorPrinter::enterSuite(
    const std::string& name_) {
  std::cout << "enter suite: " << name_ << std::endl;
}

void GeneratorPrinter::suiteStartUp() {
  std::cout << "suite start up" << std::endl;
}

void GeneratorPrinter::suiteTearDown() {
  std::cout << "suite tear down" << std::endl;
}

void GeneratorPrinter::enterCase(
    const std::string& name_) {
  std::cout << "enter case: " << name_ << std::endl;
}

void GeneratorPrinter::caseStartUp() {
  std::cout << "case start up" << std::endl;
}

void GeneratorPrinter::caseTearDown() {
  std::cout << "case tear down" << std::endl;
}

void GeneratorPrinter::enterState(
    const std::string& name_) {
  std::cout << "enter state: " << name_ << std::endl;
}

void GeneratorPrinter::appendVariable(
    const std::string& name_,
    const std::string& type_) {
  std::cout << "variable: " << name_ << " " << type_ << std::endl;
}

void GeneratorPrinter::appendVariableInit(
    const std::string& name_,
    const std::string& type_,
    const Location& ibegin_,
    const Location& iend_) {
  std::cout << "variable: " << name_ << " " << type_ << " "
      << ibegin_ << "-" << iend_ << std::endl;
}

void GeneratorPrinter::emptyBody() {
  std::cout << "empty body" << std::endl;
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

void GeneratorPrinter::endFile(
    const Location& last_) {
  std::cout << "end file " << last_ << std::endl;
}

} /* namespace OTest2 */

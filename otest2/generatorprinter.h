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

#ifndef OTest2__OTEST2_GENERATORPRINTER_H_
#define OTest2__OTEST2_GENERATORPRINTER_H_

#include "generator.h"

namespace OTest2 {

/**
 * @brief Simple generator for debug purposes
 */
class GeneratorPrinter : public Generator {
  public:
    /**
     * @brief Ctor
     */
    GeneratorPrinter();

    /**
     * @brief Dtor
     */
    virtual ~GeneratorPrinter();

    /* -- avoid copying */
    GeneratorPrinter(
        const GeneratorPrinter&) = delete;
    GeneratorPrinter& operator =(
        const GeneratorPrinter&) = delete;

    /* -- generator interface */
    virtual void beginFile();
    virtual void copySource(
        const Location& begin_,
        const Location& end_);
    virtual void enterSuite(
        const std::string& name_);
    virtual void enterCase(
        const std::string& name_);
    virtual void enterState(
        const std::string& name_);
    virtual void leaveState();
    virtual void leaveCase();
    virtual void leaveSuite();
};

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_GENERATORPRINTER_H_ */
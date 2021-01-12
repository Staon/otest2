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

#ifndef OTest2OTEST2_GENERATORSTD_H_
#define OTest2OTEST2_GENERATORSTD_H_

#include <iosfwd>

#include "generator.h"

namespace OTest2 {

class FileReader;

/**
 * @brief Standard code generator
 */
class GeneratorStd : public Generator {
  private:
    struct Impl;
    Impl* pimpl;

    /* -- avoid copying */
    GeneratorStd(
        const GeneratorStd&);
    GeneratorStd& operator =(
        const GeneratorStd&);

  public:
    /**
     * @brief Ctor
     *
     * @param output_ An output stream. The ownership is not taken.
     * @param reader_ A reader of the input file. The ownership is not taken.
     * @param domain_ Name of the testing domain. Each domain can be run
     *     separately.
     * @param infile_ Name of the input file as used in the #line directive
     * @param outfile_ Name of the output file as used in the #line directive
     */
    explicit GeneratorStd(
        std::ostream* output_,
        FileReader* reader_,
        const std::string& domain_,
        const std::string& infile_,
        const std::string& outfile_);

    /**
     * @brief Dtor
     */
    virtual ~GeneratorStd();

    /* -- generator interface */
    virtual void beginFile() override;
    virtual void startUserArea(
        const Location& begin_) override;
    virtual void copySource(
        const Location& begin_,
        const Location& end_) override;
    virtual void makeAssertion(
        const std::string& assertion_class_,
        const std::string& assertion_method_,
        const std::vector<AssertionArg>& args_ranges_) override;
    virtual void makeStateSwitch(
        const Location& state_begin_,
        const Location& state_end_,
        const Location& delay_begin_,
        const Location& delay_end_) override;
    virtual void makeTryCatchBegin(
        const Location& begin_) override;
    virtual void makeCatchHandler(
        const std::string& type_,
        const std::string& varname_) override;
    virtual void makeTryCatchEnd() override;
    virtual void endUserArea(
        const Location& end_) override;
    virtual void enterSuite(
        const std::string& name_,
        const Parser::ObjectTags& tags_) override;
    virtual void finishSuiteFixtures() override;
    virtual void finishSuiteFunctions() override;
    virtual void enterCase(
        const std::string& name_,
        const Parser::ObjectTags& tags_) override;
    virtual void finishCaseFixtures() override;
    virtual void finishCaseFunctions() override;
    virtual void enterState(
        const std::string& name_,
        FunctionPtr state_fce_,
        const Location& fbegin_,
        const Location& fend_) override;
    virtual void emptyState() override;
    virtual void appendVariable(
        const std::string& name_,
        const std::string& type_,
        InitializerPtr initializer_) override;
    virtual void appendUserData(
        const std::string& name_,
        const std::string& key_,
        const std::string& type_) override;
    virtual void appendStartUpFunction(
        FunctionPtr function_,
        const Location& fbegin_,
        const Location& fend_) override;
    virtual void appendTearDownFunction(
        FunctionPtr function_,
        const Location& fbegin_,
        const Location& fend_) override;
    virtual void appendFixtureObject(
        const std::string& name_,
        FunctionPtr start_up_,
        FunctionPtr tear_down_) override;
    virtual void appendGenericFunction(
        FunctionPtr function_,
        const Location& fbegin_,
        const Location& fend_) override;
    virtual bool appendRepeater(
        const std::string& name_,
        const std::string& type_,
        InitializerPtr initializer_) override;
    virtual void leaveState() override;
    virtual void leaveCase() override;
    virtual void leaveSuite() override;
    virtual void endFile(
        const Location& last_) override;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_GENERATORSTD_H_ */

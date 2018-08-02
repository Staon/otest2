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
     * @param infile_ Name of the input file as used in the #line directive
     * @param outfile_ Name of the output file as used in the #line directive
     */
    explicit GeneratorStd(
        std::ostream* output_,
        FileReader* reader_,
        const std::string& infile_,
        const std::string& outfile_);

    /**
     * @brief Dtor
     */
    virtual ~GeneratorStd();

    /* -- generator interface */
    virtual void beginFile();
    virtual void copySource(
        const Location& begin_,
        const Location& end_);
    virtual void enterSuite(
        const std::string& name_);
    virtual void suiteStartUp();
    virtual void suiteTearDown();
    virtual void enterCase(
        const std::string& name_);
    virtual void caseStartUp();
    virtual void caseTearDown();
    virtual void enterState(
        const std::string& name_);
    virtual void emptyBody();
    virtual void appendVariable(
        const std::string& name_,
        const std::string& type_);
    virtual void appendVariableInit(
        const std::string& name_,
        const std::string& type_,
        const Location& ibegin_,
        const Location& iend_);
    virtual void leaveState();
    virtual void leaveCase();
    virtual void leaveSuite();
    virtual void endFile(
        const Location& last_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_GENERATORSTD_H_ */

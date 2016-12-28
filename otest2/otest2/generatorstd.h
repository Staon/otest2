#ifndef OTest2OTEST2_GENERATORSTD_H_
#define OTest2OTEST2_GENERATORSTD_H_

#include <iosfwd>

#include "generator.h"

namespace OTest2 {

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
     */
    explicit GeneratorStd(
        std::ostream* output_);

    /**
     * @brief Dtor
     */
    virtual ~GeneratorStd();

    /* -- generator interface */
    virtual void beginFile();
    virtual void dumpString(
        const dstring& string_);
    virtual void enterSuite(
        const dstring& name_);
    virtual void enterState(
        const dstring& name_);
    virtual void leaveState();
    virtual void enterCase(
        const dstring& name_);
    virtual void leaveCase();
    virtual void leaveSuite();
    virtual void appendVariable(
        const dstring& name_,
        const DeclarationPtr& declaration_);
    virtual void finishDeclarations();
    virtual bool setInitializer(
        const dstring& name_,
        const dstring& body_);
    virtual void setCtorBody(
        const dstring& body_);
    virtual void setDtorBody(
        const dstring& body_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_GENERATORSTD_H_ */

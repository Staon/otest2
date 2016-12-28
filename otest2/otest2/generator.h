#ifndef OTest2OTEST2_GENERATOR_H_
#define OTest2OTEST2_GENERATOR_H_

#include <datstr/dstring.h>

#include "declaration.h"

namespace OTest2 {

/**
 * @brief Generator interface
 */
class Generator {
  private:
    /* -- avoid copying */
    Generator(
        const Generator&);
    Generator& operator =(
        const Generator&);

  public:
    /**
     * @brief Ctor
     */
    Generator();

    /**
     * @brief Dtor
     */
    virtual ~Generator();

    /**
     * @brief Generate header of the file
     */
    virtual void beginFile() = 0;

    /**
     * @brief Dump free string (parts of the source what are not processed
     *     by the preprocessor)
     *
     * @param string_ The string
     */
    virtual void dumpString(
        const dstring& string_) = 0;

    /**
     * @brief Generate beginning of the suite class
     *
     * @param name_ Name of the suite
     */
    virtual void enterSuite(
        const dstring& name_) = 0;

    /**
     * @brief Beginning of a test case
     *
     * @param name_ Name of the case
     */
    virtual void enterCase(
        const dstring& name_) = 0;

    /**
     * @brief Beginning of a test state
     *
     * @param name_ Name of the state
     */
    virtual void enterState(
        const dstring& name_) = 0;

    /**
     * @brief End of current test state
     */
    virtual void leaveState() = 0;

    /**
     * @brief End of current test case
     */
    virtual void leaveCase() = 0;

    /**
     * @brief Leave current suite
     */
    virtual void leaveSuite() = 0;

    /**
     * @brief Append new variable
     *
     * @param name_ Name of the variable
     * @param declaration_ Declaration of the variable
     */
    virtual void appendVariable(
        const dstring& name_,
        const DeclarationPtr& declaration_) = 0;

    /**
     * @brief Finish declaration section of current suite/case/state
     */
    virtual void finishDeclarations() = 0;

    /**
     * @brief Set variable initializer
     *
     * @param name_ Name of the variable
     * @param body_ Body of the initializer
     * @return True if the initializer is set. False if the variable
     *     doesn't exist.
     */
    virtual bool setInitializer(
        const dstring& name_,
        const dstring& body_) = 0;

    /**
     * @brief Set body of the ctor of current object
     */
    virtual void setCtorBody(
        const dstring& body_) = 0;

    /**
     * @brief Set body of the dtor of current object
     */
    virtual void setDtorBody(
        const dstring& body_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_GENERATOR_H_ */

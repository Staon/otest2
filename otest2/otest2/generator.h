#ifndef OTest2OTEST2_GENERATOR_H_
#define OTest2OTEST2_GENERATOR_H_

#include <datstr/dstring.h>

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
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_GENERATOR_H_ */

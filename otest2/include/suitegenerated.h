#ifndef OTest2INCLUDE_SUITEGENERATED_H_
#define OTest2INCLUDE_SUITEGENERATED_H_

#include <otest2/suite.h>

namespace OTest2 {

class Context;

/**
 * @brief Helper class for suites generated by the otest2 preprocessor
 */
class SuiteGenerated : public Suite {
  private:
    /* -- avoid copying */
    SuiteGenerated(
        const SuiteGenerated&);
    SuiteGenerated& operator =(
        const SuiteGenerated&);

  public:
    /**
     * @brief Ctor
     *
     * @param context_ OTest2 context
     */
    explicit SuiteGenerated(
        const Context& context_);

    /**
     * @brief Dtor
     */
    virtual ~SuiteGenerated();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_SUITEGENERATED_H_ */

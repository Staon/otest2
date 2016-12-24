#ifndef OTest2INCLUDE_CASEGENERATED_H_
#define OTest2INCLUDE_CASEGENERATED_H_

#include <otest2/case.h>

namespace OTest2 {

class Context;

/**
 * @brief Common implementation of a generated test case
 */
class CaseGenerated : public Case {
  private:
    /* -- avoid copying */
    CaseGenerated(
        const CaseGenerated&);
    CaseGenerated& operator =(
        const CaseGenerated&);

  public:
    /**
     * @brief Ctor
     *
     * @param context_ OTest2 context
     */
    explicit CaseGenerated(
        const Context& context_);

    /**
     * @brief Dtor
     */
    virtual ~CaseGenerated();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_CASEGENERATED_H_ */

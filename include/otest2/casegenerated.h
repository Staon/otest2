#ifndef OTest2INCLUDE_CASEGENERATED_H_
#define OTest2INCLUDE_CASEGENERATED_H_

#include <string>

#include <otest2/case.h>
#include <otest2/stateptr.h>

namespace OTest2 {

class Context;

/**
 * @brief Common implementation of a generated test case
 */
class CaseGenerated : public Case {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /* -- avoid copying */
    CaseGenerated(
        const CaseGenerated&) = delete;
    CaseGenerated& operator =(
        const CaseGenerated&) = delete;

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

  protected:
    /**
     * @brief Register new test state
     *
     * @param name_ Name of the state
     * @param state_ The state
     */
    void registerState(
        const std::string& name_,
        StatePtr state_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_CASEGENERATED_H_ */

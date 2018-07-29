#ifndef OTest2INCLUDE_CASE_H_
#define OTest2INCLUDE_CASE_H_

#include <otest2/caseptr.h>
#include <otest2/object.h>

namespace OTest2 {

class Context;

/**
 * @brief Interface of a test case
 */
class Case : public Object {
  private:
    /* -- avoid copying */
    Case(
        const Case&);
    Case& operator =(
        const Case&);

  public:
    /**
     * @brief Ctor
     */
    Case();

    /**
     * @brief Dtor
     */
    virtual ~Case();

    /**
     * @brief Schedule run of the case
     *
     * @param context_ The otest2 context
     * @param this_ptr_ A smart pointer which keeps lifetime of this case
     */
    virtual void scheduleRun(
        const Context& context_,
        CasePtr this_ptr_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_CASE_H_ */

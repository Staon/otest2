#ifndef OTest2INCLUDE_SUITE_H_
#define OTest2INCLUDE_SUITE_H_

#include <otest2/object.h>
#include <otest2/suiteptr.h>

namespace OTest2 {

class Context;

/**
 * @brief Suite interface
 */
class Suite : public Object {
  private:
    /* -- avoid copying */
    Suite(
        const Suite&);
    Suite& operator =(
        const Suite&);

  public:
    /**
     * @brief Ctor
     */
    Suite();

    /**
     * @brief Dtor
     */
    virtual ~Suite();

    /**
     * @brief Schedule run of the suite
     *
     * @param context_ The otest2 context
     * @param this_ptr_ A smart pointer which keeps lifetime of this suite
     */
    virtual void scheduleRun(
        const Context& context_,
        SuitePtr this_ptr_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_SUITE_H_ */

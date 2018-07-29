#ifndef OTest2INCLUDE_STATE_H_
#define OTest2INCLUDE_STATE_H_

#include <otest2/object.h>
#include <otest2/stateptr.h>

namespace OTest2 {

class Context;

/**
 * @brief Generic test state interface
 */
class State : public Object {
  private:
    /* -- avoid copying */
    State(
        const State&);
    State& operator =(
        const State&);

  public:
    /**
     * @brief Ctor
     */
    State();

    /**
     * @brief Dtor
     */
    virtual ~State();

    /**
     * @brief Schedule run of the state
     *
     * @param context_ The otest2 context
     * @param this_ptr_ A smart pointer which keeps lifetime of this state
     */
    virtual void scheduleRun(
        const Context& context_,
        StatePtr this_ptr_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_STATE_H_ */

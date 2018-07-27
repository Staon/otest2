#ifndef OTest2INCLUDE_STATEGENERATED_H_
#define OTest2INCLUDE_STATEGENERATED_H_

#include <otest2/state.h>

namespace OTest2 {

class Context;

/**
 * @brief Common implementation of generated states
 */
class StateGenerated : public State {
  private:

  public:
    /* -- avoid copying */
    StateGenerated(
        const StateGenerated&) = delete;
    StateGenerated& operator =(
        const StateGenerated&) = delete;

    /**
     * @brief Ctor
     *
     * @param context_ OTest2 context
     */
    explicit StateGenerated(
        const Context& context_);

    /**
     * @brief Dtor
     */
    virtual ~StateGenerated();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_STATEGENERATED_H_ */

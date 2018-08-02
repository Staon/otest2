#ifndef OTest2INCLUDE_STATEGENERATED_H_
#define OTest2INCLUDE_STATEGENERATED_H_

#include <string>

#include <otest2/contextobject.h>
#include <otest2/stateordinary.h>

namespace OTest2 {

class Context;

/**
 * @brief Common implementation of generated states
 */
class StateGenerated : public StateOrdinary, public ContextObject {
  private:
    struct Impl;
    Impl* pimpl;

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
     * @param name_ Name of the state
     */
    explicit StateGenerated(
        const Context& context_,
        const std::string& name_);

    /**
     * @brief Dtor
     */
    virtual ~StateGenerated();

    /* -- object's interface */
    virtual std::string getName() const;

    /* -- ordinary state interface */
    virtual void executeState(
        const Context& context_);

    /* -- context object */
    virtual const Context& otest2Context() const;

  private:
    /**
     * @brief The user code
     */
    virtual void runState() = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_STATEGENERATED_H_ */

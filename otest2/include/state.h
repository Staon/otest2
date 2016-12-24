#ifndef OTest2INCLUDE_STATE_H_
#define OTest2INCLUDE_STATE_H_

namespace OTest2 {

class Context;

/**
 * @brief Generic test state interface
 */
class State {
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
     * @brief Execute the state
     *
     * @param context_ OTest2 context
     */
    virtual void runState(
        const Context& context_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_STATE_H_ */

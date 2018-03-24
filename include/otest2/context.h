#ifndef OTest2INCLUDE_CONTEXT_H_
#define OTest2INCLUDE_CONTEXT_H_

namespace OTest2 {

/**
 * @brief OTest2 runtime context
 */
class Context {
  private:
    /* -- avoid copying */
    Context(
        const Context&);
    Context& operator =(
        const Context&);

  public:
    /**
     * @brief Ctor
     */
    Context();

    /**
     * @brief Dtor
     */
    ~Context();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_CONTEXT_H_ */

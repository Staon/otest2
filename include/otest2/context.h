#ifndef OTest2INCLUDE_CONTEXT_H_
#define OTest2INCLUDE_CONTEXT_H_

namespace OTest2 {

class CommandStack;
class Reporter;

/**
 * @brief OTest2 runtime context
 */
class Context {
  public:
    CommandStack* const command_stack;
    Reporter* const reporter;

    /* -- avoid copying */
    Context(
        const Context&) = delete;
    Context& operator =(
        const Context&) = delete;

    /**
     * @brief Ctor
     */
    explicit Context(
        CommandStack* command_stack_,
        Reporter* reporter_);

    /**
     * @brief Dtor
     */
    ~Context();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_CONTEXT_H_ */

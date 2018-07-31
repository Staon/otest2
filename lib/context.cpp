#include <context.h>

namespace OTest2 {

Context::Context(
    CommandStack* command_stack_,
    SemanticStack* semantic_stack_,
    Reporter* reporter_) :
  command_stack(command_stack_),
  semantic_stack(semantic_stack_),
  reporter(reporter_) {

}

Context::~Context() {

}

} /* -- namespace OTest2 */

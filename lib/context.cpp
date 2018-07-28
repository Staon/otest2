#include <context.h>

namespace OTest2 {

Context::Context(
    CommandStack* command_stack_,
    Reporter* reporter_) :
  command_stack(command_stack_),
  reporter(reporter_) {

}

Context::~Context() {

}

} /* -- namespace OTest2 */

#include <stategenerated.h>

#include <string>

#include "runcode.h"
#include <utils.h>

namespace OTest2 {

struct StateGenerated::Impl {
  public:
    StateGenerated* owner;

    std::string name;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        StateGenerated* owner_,
        const Context& context_,
        const std::string& name_);
    ~Impl();
};

StateGenerated::Impl::Impl(
    StateGenerated* owner_,
    const Context& context_,
    const std::string& name_) :
  owner(owner_),
  name(name_) {

}

StateGenerated::Impl::~Impl() {

}

StateGenerated::StateGenerated(
    const Context& context_,
    const std::string& name_) :
  StateOrdinary(context_),
  pimpl(new Impl(this, context_, name_)) {

}

StateGenerated::~StateGenerated() {
  odelete(pimpl);
}

std::string StateGenerated::getName() const {
  return pimpl->name;
}

void StateGenerated::executeState(
    const Context& context_) {
  runUserCode(context_, [this](const Context& context_) {
    runState();
  });
}

} /* -- namespace OTest2 */

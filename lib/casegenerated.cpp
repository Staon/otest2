#include <casegenerated.h>

#include <string>

#include <stateptr.h>
#include <stateregistry.h>
#include <utils.h>

namespace OTest2 {

struct CaseGenerated::Impl {
  public:
    CaseGenerated* owner;

    std::string name;
    StateRegistry state_registry;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        CaseGenerated* owner_,
        const std::string& name_);
    ~Impl();
};

CaseGenerated::Impl::Impl(
    CaseGenerated* owner_,
    const std::string& name_) :
  owner(owner_),
  name(name_),
  state_registry() {

}

CaseGenerated::Impl::~Impl() {

}

CaseGenerated::CaseGenerated(
    const Context& context_,
    const std::string& name_) :
  CaseOrdinary(context_),
  pimpl(new Impl(this, name_)) {

}

CaseGenerated::~CaseGenerated() {
  odelete(pimpl);
}

std::string CaseGenerated::getName() const {
  return pimpl->name;
}

StatePtr CaseGenerated::getFirstState() const {
  return pimpl->state_registry.getFirstState();
}

void CaseGenerated::startUpCase(
    const Context& context_) {

}

void CaseGenerated::tearDownCase(
    const Context& context_) {

}

void CaseGenerated::registerState(
    const std::string& name_,
    StatePtr state_) {
  pimpl->state_registry.registerState(name_, state_);
}

} /* -- namespace OTest2 */

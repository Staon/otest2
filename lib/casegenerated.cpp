#include <casegenerated.h>

#include <stateregistry.h>
#include <utils.h>

namespace OTest2 {

struct CaseGenerated::Impl {
  public:
    CaseGenerated* owner;

    StateRegistry state_registry;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        CaseGenerated* owner_);
    ~Impl();
};

CaseGenerated::Impl::Impl(
    CaseGenerated* owner_) :
  owner(owner_),
  state_registry() {

}

CaseGenerated::Impl::~Impl() {

}

CaseGenerated::CaseGenerated(
    const Context& context_) :
  pimpl(new Impl(this)) {

}

CaseGenerated::~CaseGenerated() {
  odelete(pimpl);
}

void CaseGenerated::registerState(
    const std::string& name_,
    StatePtr state_) {
  pimpl->state_registry.registerState(name_, state_);
}

} /* -- namespace OTest2 */

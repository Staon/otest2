#include <suitegenerated.h>

#include <caseregistry.h>

namespace OTest2 {

struct SuiteGenerated::Impl {
  public:
    SuiteGenerated* owner;

    CaseRegistry case_registry;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        SuiteGenerated* owner_);
    ~Impl();
};

SuiteGenerated::Impl::Impl(
    SuiteGenerated* owner_) :
  owner(owner_),
  case_registry() {

}

SuiteGenerated::Impl::~Impl() {

}

SuiteGenerated::SuiteGenerated(
    const Context& context_) :
  pimpl(new Impl(this)) {

}

SuiteGenerated::~SuiteGenerated() {
  delete pimpl;
  pimpl = nullptr;
}

void SuiteGenerated::registerCase(
    const std::string& name_,
    CaseFactoryPtr case_factory_) {
  pimpl->case_registry.registerCase(name_, case_factory_);
}

} /* -- namespace OTest2 */

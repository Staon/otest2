#include <suitegenerated.h>

#include <string>

#include <caseregistry.h>
#include <context.h>
#include <utils.h>

namespace OTest2 {

struct SuiteGenerated::Impl {
  public:
    SuiteGenerated* owner;

    const Context* context;
    std::string name;
    CaseRegistry case_registry;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator =(
        const Impl&) = delete;

    explicit Impl(
        SuiteGenerated* owner_,
        const Context& context_,
        const std::string& name_);
    ~Impl();
};

SuiteGenerated::Impl::Impl(
    SuiteGenerated* owner_,
    const Context& context_,
    const std::string& name_) :
  owner(owner_),
  context(&context_),
  name(name_),
  case_registry() {

}

SuiteGenerated::Impl::~Impl() {

}

SuiteGenerated::SuiteGenerated(
    const Context& context_,
    const std::string& name_) :
  SuiteOrdinary(context_),
  pimpl(new Impl(this, context_, name_)) {

}

SuiteGenerated::~SuiteGenerated() {
  odelete(pimpl);
}

std::string SuiteGenerated::getName() const {
  return pimpl->name;
}

const Context& SuiteGenerated::otest2Context() const {
  return *pimpl->context;
}

void SuiteGenerated::registerCase(
    const std::string& name_,
    CaseFactoryPtr case_factory_) {
  pimpl->case_registry.registerCase(name_, case_factory_);
}

void SuiteGenerated::startUpSuite(
    const Context& context_) {

}

void SuiteGenerated::tearDownSuite(
    const Context& context_) {

}

} /* -- namespace OTest2 */

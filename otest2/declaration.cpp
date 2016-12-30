#include "declaration.h"

#include <datstr/intrusiveptrimpl.h>

#include "declreference.h"

template class cIntrusivePtrFull< ::OTest2::Declaration >;

namespace OTest2 {

Declaration::Declaration() {

}

Declaration::~Declaration() {

}

void Declaration::incRef() {
  refcount.incRef();
}

void Declaration::decRef() {
  if(!refcount.decRef())
    delete this;
}

DeclarationPtr Declaration::makeReference() {
  DeclReference* ref_(new DeclReference);
  ref_ -> applyLeftDecl(this);
  return ref_;
}

} /* -- namespace OTest2 */

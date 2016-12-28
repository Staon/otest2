#include "declcore.h"

#include <datstr/intrusiveptrimpl.h>
#include <iostream>
#include <ondrart/oassert.h>

#include "declarray.h"
#include "declfunction.h"

template class cIntrusivePtrFull< ::OTest2::DeclCore >;

namespace OTest2 {

DeclCore::DeclCore() {

}

DeclCore::DeclCore(
    const dstring& name_) :
  name(name_) {
  OASSERT_1(!name.IsNullEmpty());

}

DeclCore::~DeclCore() {

}

void DeclCore::incRef() {
  refcount.incRef();
}

void DeclCore::decRef() {
  if(!refcount.decRef())
    delete this;
}

void DeclCore::applyRightDecl(
    Declaration* decl_) {
  OASSERT_1(decl_ != 0);

  if(!decl)
    decl = decl_;
  else
    decl -> applyRightDecl(decl_);
}

std::ostream& DeclCore::printDeclaration(
    std::ostream& os_) const {
  decl -> printLeftDeclaration(os_);
  if(!name.IsNull())
    os_ << name << ' ';
  decl -> printRightDeclaration(os_);
  return os_;
}

} /* -- namespace OTest2 */

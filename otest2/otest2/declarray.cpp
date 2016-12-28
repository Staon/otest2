#include "declarray.h"

#include <iostream>
#include <ondrart/oassert.h>

namespace OTest2 {

DeclArray::DeclArray(
    const dstring& size_,
    Declaration* decl_) :
  size(size_),
  decl(decl_) {
  OASSERT_1(!size_.IsNull());
  OASSERT_1(!!decl);

}

DeclArray::~DeclArray() {

}

void DeclArray::applyRightDecl(
    Declaration* decl_) {
  OASSERT_1(decl_ != 0);
  decl -> applyRightDecl(decl_);
}

bool DeclArray::isRightAggressive() const {
  return true;
}

void DeclArray::applyModifiersDeep(
    std::uint8_t modifier_) {
  decl -> applyModifiersDeep(modifier_);
}

void DeclArray::printLeftDeclaration(
    std::ostream& os_) {
  decl -> printLeftDeclaration(os_);
}

void DeclArray::printRightDeclaration(
    std::ostream& os_) {
  os_ << '[' << size << ']';
  decl -> printRightDeclaration(os_);
}

} /* -- namespace OTest2 */

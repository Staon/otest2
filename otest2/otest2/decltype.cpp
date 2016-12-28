#include "decltype.h"

#include <iostream>
#include <ondrart/oassert.h>

namespace OTest2 {

DeclType::DeclType(
    const dstring& type_) :
  type(type_) {
  OASSERT_1(!type.IsNullEmpty());

}

DeclType::~DeclType() {

}

bool DeclType::isChildRightAggressive() const {
  return false;
}

void DeclType::printLeftBody(
    std::ostream& os_,
    bool print_parenthesis_) {
  os_ << type << ' ';
}

void DeclType::printRightBody(
    std::ostream& os_) {

}

void DeclType::applyRightDecl(
    Declaration* decl_) {
  OASSERT_NEVER();
}

void DeclType::applyModifiersDeep(
    std::uint8_t modifier_) {
  applyModifiers(modifier_);
}

void DeclType::applyLeftDecl(
    DeclLeft* decl_) {
  OASSERT_NEVER();
}

} /* -- namespace OTest2 */

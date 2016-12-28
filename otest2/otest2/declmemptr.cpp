#include "declmemptr.h"

#include <iostream>
#include <ondrart/oassert.h>

namespace OTest2 {

DeclMemPtr::DeclMemPtr(
    const dstring& type_) :
  type(type_) {
  OASSERT_1(!type.IsNullEmpty());

}

DeclMemPtr::~DeclMemPtr() {

}

bool DeclMemPtr::isChildRightAggressive() const {
  return decl -> isRightAggressive();
}

void DeclMemPtr::printLeftBody(
    std::ostream& os_,
    bool print_parenthesis_) {
  decl -> printLeftDeclaration(os_);
  if(print_parenthesis_)
    os_ << '(';
  os_ << type << "::* ";
}

void DeclMemPtr::printRightBody(
    std::ostream& os_) {
  decl -> printRightDeclaration(os_);
}

void DeclMemPtr::applyRightDecl(
    Declaration* decl_) {
  OASSERT_1(decl_ != 0);
  if(!decl)
    decl = decl_;
  else
    decl -> applyRightDecl(decl_);
}

void DeclMemPtr::applyModifiersDeep(
    std::uint8_t modifier_) {
  if(!decl)
    applyModifiers(modifier_);
  else
    decl -> applyModifiersDeep(modifier_);
}

void DeclMemPtr::applyLeftDecl(
    Declaration* decl_) {
  OASSERT_1(!decl && decl_ != 0);
  decl = decl_;
}

} /* -- namespace OTest2 */

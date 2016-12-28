#include "declreference.h"

#include <iostream>
#include <ondrart/oassert.h>

namespace OTest2 {

DeclReference::DeclReference() {

}

DeclReference::~DeclReference() {

}

bool DeclReference::isChildRightAggressive() const {
  return decl -> isRightAggressive();
}

void DeclReference::printLeftBody(
    std::ostream& os_,
    bool print_parenthesis_) {
  decl -> printLeftDeclaration(os_);
  if(print_parenthesis_)
    os_ << '(';
  os_ << "& ";
}

void DeclReference::printRightBody(
    std::ostream& os_) {
  decl -> printRightDeclaration(os_);
}

void DeclReference::applyRightDecl(
    Declaration* decl_) {
  OASSERT_1(decl_ != 0);
  if(!decl)
    decl = decl_;
  else
    decl -> applyRightDecl(decl_);
}

void DeclReference::applyModifiersDeep(
    std::uint8_t modifier_) {
  if(!decl)
    applyModifiers(modifier_);
  else
    decl -> applyModifiersDeep(modifier_);
}

DeclarationPtr DeclReference::makeReference() {
  return this;
}

void DeclReference::applyLeftDecl(
    Declaration* decl_) {
  OASSERT_1(!decl && decl_ != 0);
  decl = decl_;
}

} /* -- namespace OTest2 */

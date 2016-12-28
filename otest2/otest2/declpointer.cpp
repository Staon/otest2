#include "declpointer.h"

#include <iostream>
#include <ondrart/oassert.h>

namespace OTest2 {

DeclPointer::DeclPointer() {

}

DeclPointer::~DeclPointer() {

}

bool DeclPointer::isChildRightAggressive() const {
  return decl -> isRightAggressive();
}

void DeclPointer::printLeftBody(
    std::ostream& os_,
    bool print_parenthesis_) {
  decl -> printLeftDeclaration(os_);
  if(print_parenthesis_)
    os_ << '(';
  os_ << "* ";
}

void DeclPointer::printRightBody(
    std::ostream& os_) {
  decl -> printRightDeclaration(os_);
}

void DeclPointer::applyRightDecl(
    Declaration* decl_) {
  OASSERT_1(decl_ != 0);
  if(!decl)
    decl = decl_;
  else
    decl -> applyRightDecl(decl_);
}

void DeclPointer::applyModifiersDeep(
    std::uint8_t modifier_) {
  if(!decl)
    applyModifiers(modifier_);
  else
    decl -> applyModifiersDeep(modifier_);
}

void DeclPointer::applyLeftDecl(
    Declaration* decl_) {
  OASSERT_1(!decl && decl_ != 0);
  decl = decl_;
}

} /* -- namespace OTest2 */

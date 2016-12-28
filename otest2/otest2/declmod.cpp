#include "declmod.h"

#include <iostream>

namespace OTest2 {

DeclMod::DeclMod() :
  modifier(0),
  print_parenthesis(false) {

}

DeclMod::~DeclMod() {

}

bool DeclMod::isRightAggressive() const {
  return false;
}

void DeclMod::printLeftDeclaration(
    std::ostream& os_) {
  print_parenthesis = isChildRightAggressive();
  printLeftBody(os_, print_parenthesis);
  if(modifier & Declaration::CONST)
    os_ << "const ";
}

void DeclMod::printRightDeclaration(
    std::ostream& os_) {
  if(print_parenthesis)
    os_ << ')';
  printRightBody(os_);
}

void DeclMod::applyModifiers(
    std::uint8_t modifier_) {
  modifier |= modifier_;
}

} /* -- namespace OTest2 */

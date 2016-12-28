#include "declfunction.h"

#include <iostream>
#include <ondrart/oassert.h>

#include "declargs.h"

namespace OTest2 {

DeclFunction::DeclFunction(
    Declaration* rettype_,
    DeclArgs* arguments_) :
  rettype(rettype_),
  arguments(arguments_) {
  OASSERT_1(!!rettype && !!arguments);

}

DeclFunction::~DeclFunction() {

}

void DeclFunction::applyRightDecl(
    Declaration* decl_) {
  OASSERT_1(decl_ != 0);
  rettype -> applyRightDecl(decl_);
}

bool DeclFunction::isRightAggressive() const {
  return true;
}

void DeclFunction::applyModifiersDeep(
    std::uint8_t modifier_) {
  rettype -> applyModifiersDeep(modifier_);
}

void DeclFunction::printLeftDeclaration(
    std::ostream& os_) {
  rettype -> printLeftDeclaration(os_);
}

void DeclFunction::printRightDeclaration(
    std::ostream& os_) {
  os_ << '(';
  arguments -> printRightDeclaration(os_);
  os_ << ')';
  rettype -> printRightDeclaration(os_);
}

} /* -- namespace OTest2 */

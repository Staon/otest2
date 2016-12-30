#include "declargs.h"

#include <iostream>
#include <ondrart/oassert.h>

#include "declcore.h"

namespace OTest2 {

DeclArgs::DeclArgs() {

}

DeclArgs::~DeclArgs() {

}

void DeclArgs::applyRightDecl(
    Declaration* decl_) {
  OASSERT_NEVER();
}

bool DeclArgs::isRightAggressive() const {
  OASSERT_NEVER();
  return false;
}

void DeclArgs::applyModifiersDeep(
    std::uint8_t modifier_) {
  OASSERT_NEVER();
}

void DeclArgs::printLeftDeclaration(
    std::ostream& os_) {
  OASSERT_NEVER();
}

void DeclArgs::printRightDeclaration(
    std::ostream& os_) {
  bool first_(true);
  for(
      Arguments::const_iterator iter_(arguments.begin());
      iter_ != arguments.end();
      ++iter_) {
    if(first_)
      first_ = false;
    else
      os_ << ", ";
    (*iter_) -> printDeclaration(os_);
  }
}

void DeclArgs::appendArgument(
    DeclCore* arg_) {
  OASSERT_1(arg_ != 0);
  arguments.push_back(arg_);
}

} /* -- namespace OTest2 */

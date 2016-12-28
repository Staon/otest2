#include "vartable.h"

#include <algorith>
#include <astraios/dstream.h>
#include <datstr/intrusiveptrimpl.h>
#include <ondrart/oassert.h>

#include "declcore.h"

template class cIntrusivePtrFull< ::OTest2::VarTable >;

namespace OTest2 {

namespace {

void printIndent(
    std::ostream& os_,
    int indent_) {
  for(; indent_ > 0; --indent_)
    os_ << "  ";
}

} /* -- namespace */

VarTable::VarTable(
    const dstring& name_,
    VarTable* level_) :
  refcount(),
  name(name_),
  level(level_),
  variables() {
  OASSERT_1(!name.IsNullEmpty());

  if(level.IsNotNull()) {
    /* -- copy variables from previous level */
    for(
        Order::const_iterator iter_(level -> order.begin());
        iter_ != level -> order.end();
        ++iter_) {
      /* -- copy to the order list */
      order.push_back(*iter_);

      /* -- copy to the table */
      Variables::const_iterator var_(level -> variables.find(*iter_));
      Record record_;
      record_.mine = false;
      record_.declaration = (*var_).second.declaration -> makeReference();
      dstrostream dos_(record_.initializer);
      dos_ << level -> getName() << " -> " << *iter_;
      dos_.str();
      variables.insert(Variables::value_type(*iter_, record_));
    }
  }
}

VarTable::~VarTable() {

}

void VarTable::incRef() {
  refcount.incRef();
}

void VarTable::decRef() {
  if(!refcount.decRef())
    delete this;
}

const dstring& VarTable::getName() const {
  return name;
}

const VarTablePtr& VarTable::getPrevLevel() const {
  return level;
}

void VarTable::appendVariable(
    const dstring& name_,
    const DeclarationPtr& declaration_) {
  OASSERT_1(!name_.IsNullEmpty() && declaration_.IsNotNull());

  Variables::iterator iter_(variables.find(name_));
  if(iter_ == variables.end()) {
    /* -- new item */
    Record record_;
    record_.mine = true;
    record_.declaration = declaration_;
    variables.insert(Variables::value_type(name_, record_));
    order.push_back(name_);
  }
  else {
    /* -- overwrite */
    (*iter_).second.mine = true;
    (*iter_).second.declaration = declaration_;
    (*iter_).second.initializer.SetNull();
    order.erase(std::remove(order.begin(), order.end(), name_));
    order.push_back(name_);
  }
}

bool VarTable::setInitializer(
    const dstring& name_,
    const dstring& body_) {
  OASSERT_1(!name_.IsNullEmpty() && !body_.IsNull());

  Variables::iterator iter_(variables.find(name_));
  if(iter_ != variables.end() && (*iter_).second.mine) {
    (*iter_).second.initializer = body_;
    return true;
  }
  else
    return false;
}

void VarTable::printDeclarations(
    std::ostream& os_,
    int indent_) const {
  for(
      Order::const_iterator iter_(order.begin());
      iter_ != order.end();
      ++iter_) {
    Variables::const_iterator var_(variables.find(*iter_));

    printIndent(os_, indent_);
    DeclCorePtr core_(new DeclCore(*iter_));
    core_ -> applyRightDecl((*var_).second.declaration.ObjectAddr());
    core_ -> printDeclaration(os_);
    os_ << ";\n";
  }
}

void VarTable::printInitializers(
    std::ostream& os_,
    int indent_) const {
  bool first_(true);
  for(
      Order::const_iterator iter_(order.begin());
      iter_ != order.end();
      ++iter_) {
    Variables::const_iterator var_(variables.find(*iter_));

    if(!(*var_).second.initializer.IsNullEmpty()) {
      os_ << ",\n";
      printIndent(os_, indent_);
      os_ << *iter_ << '(';
      os_ << (*var_).second.initializer;
      os_ << ')';
    }
  }
}

} /* -- namespace OTest2 */

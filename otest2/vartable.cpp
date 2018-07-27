#include "vartable.h"

#include <algorithm>
#include <assert.h>
#include <ostream>
#include <sstream>

#include "formatting.h"

namespace OTest2 {

VarTable::VarTable(
    const std::string& name_,
    VarTablePtr level_) :
  name(name_),
  level(level_),
  variables() {
  assert(!name.empty());

  if(level != nullptr) {
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
      record_.declaration = (*var_).second.declaration;
      variables.insert(Variables::value_type(*iter_, record_));
    }
  }
}

VarTable::~VarTable() {

}

const std::string& VarTable::getName() const {
  return name;
}

VarTablePtr VarTable::getPrevLevel() const {
  return level;
}

void VarTable::appendVariable(
    const std::string& name_,
    const std::string& declaration_) {
  appendVariableWithInit(name_, declaration_, "");
}

void VarTable::appendVariableWithInit(
    const std::string& name_,
    const std::string& declaration_,
    const std::string& initializer_) {
  assert(!name_.empty());

  Variables::iterator iter_(variables.find(name_));
  if(iter_ == variables.end()) {
    /* -- new item */
    Record record_;
    record_.mine = true;
    record_.declaration = declaration_;
    record_.initializer = initializer_;
    variables.insert(Variables::value_type(name_, record_));
    order.push_back(name_);
  }
  else {
    /* -- overwrite */
    (*iter_).second.mine = true;
    (*iter_).second.declaration = declaration_;
    (*iter_).second.initializer = initializer_;
    order.erase(std::remove(order.begin(), order.end(), name_));
    order.push_back(name_);
  }
}

void VarTable::printDeclarations(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : order) {
    Variables::const_iterator var_(variables.find(name_));

    Formatting::printIndent(os_, indent_);
    if((*var_).second.mine) {
      os_ << "typename ::OTest2::TypeOfMine<" << (*var_).second.declaration
          << ">::Type " << name_ << ";\n";
    }
    else {
      os_ << "typename ::OTest2::TypeOfParent<" << (*var_).second.declaration
          << ">::Type " << name_ << ";\n";
    }
  }
}

void VarTable::printInitializers(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : order) {
    Variables::const_iterator var_(variables.find(name_));

    os_ << ",\n";
    Formatting::printIndent(os_, indent_);
    if((*var_).second.mine) {
      os_ << name_ << '(';
      os_ << (*var_).second.initializer;
      os_ << ')';
    }
    else
      os_ << name_ << '(' << name_ << "_)";
  }
}

void VarTable::printParameters(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : order) {
    Variables::const_iterator var_(variables.find(name_));
    if(!(*var_).second.mine) {
      os_ << ",\n";
      Formatting::printIndent(os_, indent_);
      os_ << "typename ::OTest2::TypeOfParent<" << (*var_).second.declaration
          << ">::Type " << name_ << "_";

    }
  }
}

void VarTable::printArguments(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : order) {
    Variables::const_iterator var_(variables.find(name_));
    if(!(*var_).second.mine) {
      os_ << ",\n";
      Formatting::printIndent(os_, indent_);
      os_ << name_;

    }
  }
}

} /* -- namespace OTest2 */

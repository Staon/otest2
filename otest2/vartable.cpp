/*
 * Copyright (C) 2018 Ondrej Starek
 *
 * This file is part of OTest2.
 *
 * OTest2 is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * OTest2 is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OTest2.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vartable.h"

#include <algorithm>
#include <assert.h>
#include <ostream>
#include <otest2/utils.h>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "formatting.h"
#include "generfmt.h"

namespace OTest2 {

namespace {

class Variable {
  public:
    virtual ~Variable() = default;

    virtual void printType(
        std::ostream& os_) const = 0;
    virtual void printDeclaration(
        std::ostream& os_,
        const std::string& name_) const = 0;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const = 0;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const = 0;
    virtual void printArgument(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const = 0;
};

typedef std::shared_ptr<Variable> VariablePtr;

class VariableMine : public Variable {
  private:
    std::string declaration;
    std::string initializer;

  public:
    explicit VariableMine(
        const std::string& declaration_,
        const std::string& initializer_);
    virtual void printType(
        std::ostream& os_) const override;
    virtual void printDeclaration(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printArgument(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
};

class VariableInherited : public Variable {
  private:
    VariablePtr parent;

  public:
    explicit VariableInherited(
        VariablePtr parent_);
    virtual void printType(
        std::ostream& os_) const override;
    virtual void printDeclaration(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printArgument(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
};

class VariableUserData : public Variable {
  private:
    std::string key;
    std::string declaration;

  public:
    explicit VariableUserData(
        const std::string& key_,
        const std::string& declaration_);
    virtual void printType(
        std::ostream& os_) const override;
    virtual void printDeclaration(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printArgument(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
};

VariableMine::VariableMine(
    const std::string& declaration_,
    const std::string& initializer_) :
  declaration(declaration_),
  initializer(initializer_) {
  assert(!declaration.empty());

}

void VariableMine::printType(
    std::ostream& os_) const {
  os_ << declaration;
}

void VariableMine::printDeclaration(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << "typename ::OTest2::TypeOfMine<";
  printType(os_);
  os_ << ">::Type " << name_;
}

void VariableMine::printInitializer(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << initializer;
}

void VariableMine::printParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - my variable is not passed from the parent */
}

void VariableMine::printArgument(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - my variable is not passed from the parent */
}

VariableInherited::VariableInherited(
    VariablePtr parent_) :
  parent(parent_) {
  assert(parent != nullptr);

}

void VariableInherited::printType(
    std::ostream& os_) const {
  parent->printType(os_);
}

void VariableInherited::printDeclaration(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << "typename ::OTest2::TypeOfParent<";
  printType(os_);
  os_ << ">::Type " << name_;
}

void VariableInherited::printInitializer(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << name_ << "_";
}

void VariableInherited::printParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  os_ << ",\n";
  Formatting::printIndent(os_, indent_);
  os_ << "typename ::OTest2::TypeOfParent<";
  printType(os_);
  os_ << ">::Type " << name_ << "_";
}

void VariableInherited::printArgument(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  os_ << ",\n";
  Formatting::printIndent(os_, indent_);
  os_ << name_;
}

VariableUserData::VariableUserData(
    const std::string& key_,
    const std::string& declaration_) :
  key(key_),
  declaration(declaration_) {
  assert(!key.empty());
  assert(!declaration.empty());

}

void VariableUserData::printType(
    std::ostream& os_) const {
  os_ << declaration;
}

void VariableUserData::printDeclaration(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << "typename ::OTest2::TypeOfParent<";
  printType(os_);
  os_ << ">::Type " << name_;
}

void VariableUserData::printInitializer(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << "context_.user_data->getDatum<" << declaration << ">(";
  writeCString(os_, key);
  os_ << ")";
}

void VariableUserData::printParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - the user datum is not passed from the parent */
}

void VariableUserData::printArgument(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - the user datum is not passed from the parent */
}

} /* -- namespace */

struct VarTable::Impl {
    std::string name;
    VarTablePtr level;
    typedef std::unordered_map<std::string, VariablePtr> Variables;
    Variables variables;
    typedef std::vector<std::string> Order;
    Order order;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& name_,
        VarTablePtr level_);
    ~Impl() = default;

    void appendVariable(
        const std::string& name_,
        VariablePtr variable_);
};

VarTable::Impl::Impl(
    const std::string& name_,
    VarTablePtr level_) :
  name(name_),
  level(level_),
  variables()  {
  assert(!name.empty());

  if(level != nullptr) {
    /* -- copy variables from previous level */
    for(
        Order::const_iterator iter_(level->pimpl->order.begin());
        iter_ != level->pimpl->order.end();
        ++iter_) {
      order.push_back(*iter_);
      Variables::const_iterator var_(level->pimpl->variables.find(*iter_));
      assert(var_ != variables.end());
      variables.insert(
          Variables::value_type(
              *iter_, std::make_unique<VariableInherited>((*var_).second)));
    }
  }
}

void VarTable::Impl::appendVariable(
    const std::string& name_,
    VariablePtr variable_) {
  assert(!name_.empty());

  /* -- erase old record if the variable already exists */
  Impl::Variables::iterator iter_(variables.find(name_));
  if(iter_ != variables.end()) {
    variables.erase(iter_);
    order.erase(std::remove(order.begin(), order.end(), name_));
  }

  /* -- insert the new item */
  variables.insert(Impl::Variables::value_type(name_, variable_));
  order.push_back(name_);
}

VarTable::VarTable(
    const std::string& name_,
    VarTablePtr level_) :
  pimpl(new Impl(name_, level_)) {
}

VarTable::~VarTable() {
  odelete(pimpl);
}

const std::string& VarTable::getName() const {
  return pimpl->name;
}

VarTablePtr VarTable::getPrevLevel() const {
  return pimpl->level;
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
  pimpl->appendVariable(
      name_,
      std::make_shared<VariableMine>(declaration_, initializer_));
}

void VarTable::appendUserData(
    const std::string& name_,
    const std::string& key_,
    const std::string& declaration_) {
  pimpl->appendVariable(
      name_,
      std::make_shared<VariableUserData>(key_, declaration_));
}

void VarTable::printDeclarations(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : pimpl->order) {
    Impl::Variables::const_iterator var_(pimpl->variables.find(name_));

    Formatting::printIndent(os_, indent_);
    (*var_).second->printDeclaration(os_, name_);
    os_ << ";\n";
  }
}

void VarTable::printInitializers(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : pimpl->order) {
    Impl::Variables::const_iterator var_(pimpl->variables.find(name_));

    os_ << ",\n";
    Formatting::printIndent(os_, indent_);
    os_ << name_ << '(';
    (*var_).second->printInitializer(os_, name_);
    os_ << ')';
  }
}

void VarTable::printParameters(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : pimpl->order) {
    Impl::Variables::const_iterator var_(pimpl->variables.find(name_));
    (*var_).second->printParameter(os_, indent_, name_);
  }
}

void VarTable::printArguments(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : pimpl->order) {
    Impl::Variables::const_iterator var_(pimpl->variables.find(name_));
    (*var_).second->printArgument(os_, indent_, name_);
  }
}

} /* -- namespace OTest2 */

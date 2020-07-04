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
    virtual void printParentName(
        std::ostream& os_,
        const std::string& name_) const;
    virtual void printDeclaration(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const = 0;
    virtual void printInvoker(
        std::ostream& os_,
        int indent_,
        const std::string& name_,
        const std::string& classname_) const = 0;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const = 0;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const = 0;
    virtual void printFactoryParameter(
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
    InitializerPtr initializer;

  public:
    explicit VariableMine(
        const std::string& declaration_,
        InitializerPtr initializer_);
    virtual void printType(
        std::ostream& os_) const override;
    virtual void printDeclaration(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printInvoker(
        std::ostream& os_,
        int indent_,
        const std::string& name_,
        const std::string& classname_) const override;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printFactoryParameter(
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
        int indent_,
        const std::string& name_) const override;
    virtual void printInvoker(
        std::ostream& os_,
        int indent_,
        const std::string& name_,
        const std::string& classname_) const override;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printFactoryParameter(
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
        int indent_,
        const std::string& name_) const override;
    virtual void printInvoker(
        std::ostream& os_,
        int indent_,
        const std::string& name_,
        const std::string& classname_) const override;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printFactoryParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printArgument(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
};

class VariableFunction : public Variable {
  private:
    FunctionPtr function;

  public:
    explicit VariableFunction(
        FunctionPtr function_);
    virtual void printType(
        std::ostream& os_) const override;
    virtual void printParentName(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printDeclaration(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printInvoker(
        std::ostream& os_,
        int indent_,
        const std::string& name_,
        const std::string& classname_) const override;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printFactoryParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printArgument(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
};

class VariableRepeater : public Variable {
  private:
    std::string type;
    InitializerPtr initializer;

  public:
    explicit VariableRepeater(
        const std::string& type_,
        InitializerPtr initializer_);
    virtual void printType(
        std::ostream& os_) const override;
    virtual void printDeclaration(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printInvoker(
        std::ostream& os_,
        int indent_,
        const std::string& name_,
        const std::string& classname_) const override;
    virtual void printInitializer(
        std::ostream& os_,
        const std::string& name_) const override;
    virtual void printParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printFactoryParameter(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
    virtual void printArgument(
        std::ostream& os_,
        int indent_,
        const std::string& name_) const override;
};

void Variable::printParentName(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << name_;
}

VariableMine::VariableMine(
    const std::string& declaration_,
    InitializerPtr initializer_) :
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
    int indent_,
    const std::string& name_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "typename ::OTest2::TypeOfMine<";
  printType(os_);
  os_ << ">::Type " << name_ << ";\n";
}

void VariableMine::printInvoker(
    std::ostream& os_,
    int indent_,
    const std::string& name_,
    const std::string& classname_) const {
  /* -- nothing to do - variable doesn't have an invoker */
}

void VariableMine::printInitializer(
    std::ostream& os_,
    const std::string& name_) const {
  if(initializer != nullptr)
    initializer->printVarInitialization(os_, name_);
  else
    os_ << name_ << "()";
}

void VariableMine::printParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - my variable is not passed from the parent */
}

void VariableMine::printFactoryParameter(
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
    int indent_,
    const std::string& name_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "typename ::OTest2::TypeOfParent<";
  printType(os_);
  os_ << ">::Type " << name_ << ";\n";
}

void VariableInherited::printInvoker(
    std::ostream& os_,
    int indent_,
    const std::string& name_,
    const std::string& classname_) const {
  /* -- nothing to do - variable doesn't have an invoker */
}

void VariableInherited::printInitializer(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << name_ << '(' << name_ << "_)";
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

void VariableInherited::printFactoryParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - the factory method access the members directly */
}

void VariableInherited::printArgument(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  os_ << ",\n";
  Formatting::printIndent(os_, indent_);
  parent->printParentName(os_, name_);
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
    int indent_,
    const std::string& name_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "typename ::OTest2::TypeOfParent<";
  printType(os_);
  os_ << ">::Type " << name_ << ";\n";
}

void VariableUserData::printInvoker(
    std::ostream& os_,
    int indent_,
    const std::string& name_,
    const std::string& classname_) const {
  /* -- nothing to do - variable doesn't have an invoker */
}

void VariableUserData::printInitializer(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << name_ << "(" << "context_.user_data->getDatum<" << declaration << ">(";
  writeCString(os_, key);
  os_ << "))";
}

void VariableUserData::printParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - the user datum is not passed from the parent */
}

void VariableUserData::printFactoryParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - my variable is not passed from the parent */
}

void VariableUserData::printArgument(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - the user datum is not passed from the parent */
}

VariableFunction::VariableFunction(
    FunctionPtr function_) :
  function(function_) {

}

void VariableFunction::printType(
    std::ostream& os_) const {
  os_ << "const " << function->getInvokerClass() << "&";
}

void VariableFunction::printParentName(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << function->getInvokerName();
}

void VariableFunction::printDeclaration(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - function has an invoker */
}

void VariableFunction::printInvoker(
    std::ostream& os_,
    int indent_,
    const std::string& name_,
    const std::string& classname_) const {
  os_ << "\n";
  function->generateInvoker(os_, indent_, classname_);
  os_ << "\n";
}

void VariableFunction::printInitializer(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << function->getInvokerName() << "(this)";
}

void VariableFunction::printParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - function is not passed from the parent */
}

void VariableFunction::printFactoryParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - the factory method access the invoker directly */
}

void VariableFunction::printArgument(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- nothing to do - function is not passed from the parent */
}

VariableRepeater::VariableRepeater(
    const std::string& type_,
    InitializerPtr initializer_) :
  type(type_),
  initializer(initializer_) {

}

void VariableRepeater::printType(
    std::ostream& os_) const {
  os_ << type;
}

void VariableRepeater::printDeclaration(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "typename ::OTest2::TypeOfParent<";
  printType(os_);
  os_ << ">::Type " << name_ << ";\n";
}

void VariableRepeater::printInvoker(
    std::ostream& os_,
    int indent_,
    const std::string& name_,
    const std::string& classname_) const {
  os_ << "\n";

  /* -- declaration of the initialization function */
  Formatting::printIndent(os_, indent_);
  os_ << "typename ::OTest2::TypeOfParent<";
  printType(os_);
  os_ << ">::Type " << "repeaterInit_" << name_ << "(\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "const ::OTest2::Context& context_,\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "std::shared_ptr<";
  printType(os_);
  os_ << ">& current_) {\n";

  /* -- invoke the factory method */
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "current_ = ";
  printType(os_);
  os_ << "::createNext(context_, current_";
  if(initializer != nullptr) {
    os_ << ", ";
    initializer->printArgInitialization(os_, name_);
  }
  os_ << ");\n";

  /* -- return the reference to the created object */
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "return *current_;\n";

  Formatting::printIndent(os_, indent_);
  os_ << "}\n";
}

void VariableRepeater::printInitializer(
    std::ostream& os_,
    const std::string& name_) const {
  os_ << name_ << "(repeaterInit_" << name_ << "(context_, " << name_ << "_))";
}

void VariableRepeater::printParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  os_ << ",\n";
  Formatting::printIndent(os_, indent_);
  os_ << "std::shared_ptr<";
  printType(os_);
  os_ << ">& " << name_ << "_";
}

void VariableRepeater::printFactoryParameter(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  /* -- the repeater must be passed from the object factory */
  os_ << ",\n";
  Formatting::printIndent(os_, indent_);
  os_ << "std::shared_ptr<";
  printType(os_);
  os_ << ">& " << name_ << "_";
}

void VariableRepeater::printArgument(
    std::ostream& os_,
    int indent_,
    const std::string& name_) const {
  os_ << ",\n";
  Formatting::printIndent(os_, indent_);
  os_ << name_ << "_";
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
    const std::string& declaration_,
    InitializerPtr initializer_) {
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

void VarTable::appendUserFunction(
    FunctionPtr function_) {
  assert(function_ != nullptr);

  pimpl->appendVariable(
      function_->getName(),
      std::make_shared<VariableFunction>(function_));
}

void VarTable::appendRepeater(
    const std::string& name_,
    const std::string& declaration_,
    InitializerPtr initializer_) {
  pimpl->appendVariable(
      name_,
      std::make_shared<VariableRepeater>(declaration_, initializer_));
}

void VarTable::printDeclarations(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : pimpl->order) {
    Impl::Variables::const_iterator var_(pimpl->variables.find(name_));
    (*var_).second->printDeclaration(os_, indent_, name_);
  }
}

void VarTable::printInvokers(
    std::ostream& os_,
    int indent_,
    const std::string& classname_) const {
  for(const std::string& name_ : pimpl->order) {
    Impl::Variables::const_iterator var_(pimpl->variables.find(name_));
    (*var_).second->printInvoker(os_, indent_, name_, classname_);
  }
}

void VarTable::printInitializers(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : pimpl->order) {
    Impl::Variables::const_iterator var_(pimpl->variables.find(name_));
    os_ << ",\n";
    Formatting::printIndent(os_, indent_);
    (*var_).second->printInitializer(os_, name_);
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

void VarTable::printFactoryParameters(
    std::ostream& os_,
    int indent_) const {
  for(const std::string& name_ : pimpl->order) {
    Impl::Variables::const_iterator var_(pimpl->variables.find(name_));
    (*var_).second->printFactoryParameter(os_, indent_, name_);
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

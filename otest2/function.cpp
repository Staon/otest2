/*
 * Copyright (C) 2020 Ondrej Starek
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

#include "function.h"

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

#include "formatting.h"
#include "generfmt.h"

namespace OTest2 {

namespace {

std::string marshalerName(
    FunctionAccess access_,
    const std::string& instance_,
    const std::string& clash_,
    const std::string& fce_name_) {
  std::ostringstream oss_;
  oss_ << "Marshaler_";
  if(access_ != FunctionAccess::NONE)
    oss_ << instance_ << "_";
  if(!clash_.empty())
    oss_ << clash_ << "_";
  oss_ << fce_name_;
  return oss_.str();
}

std::string invokerClass(
    const std::string& fce_name_) {
  return "Invoker_" + fce_name_;
}

std::string invokerName(
    const std::string& fce_name_) {
  return "invoker_" + fce_name_ + "_object";
}

} /* -- namespace */

Function::Function(
    FunctionAccess access_,
    const std::string& instance_,
    const std::string& clash_,
    const std::string& name_,
    const std::string& rettype_) :
  access(access_),
  instance(instance_),
  clash(clash_),
  name(name_),
  rettype(rettype_) {
  assert(access == FunctionAccess::NONE || !instance.empty());
  assert(!name.empty() && !rettype.empty());

}

Function::~Function() {

}

void Function::generateFceParameters(
    std::ostream& os_,
    int indent_) const {
  bool first_(true);
  for(const auto& param_ : parameters) {
    if(!first_)
      os_ << ",";
    else
      first_ = false;
    os_ << "\n";
    Formatting::printIndent(os_, indent_);
    switch(param_.param_type) {
      case USER_DATUM:
        os_ << "typename ::OTest2::TypeTrait< " << param_.type << " >::BestArg " << param_.name;
        break;

      case CONTEXT:
        os_ << "const ::OTest2::Context& " << param_.name;
        break;

      default:
        assert("invalid parameter type" == nullptr);
        break;
    }
  }
}

void Function::generateFceArguments(
    std::ostream& os_,
    int indent_) const {
  bool first_(true);
  for(const auto& param_ : parameters) {
    if(!first_)
      os_ << ",";
    else
      first_ = false;
    os_ << "\n";
    Formatting::printIndent(os_, indent_);
    switch(param_.param_type) {
      case USER_DATUM:
        os_ << "context_.user_data->getDatum<" << param_.type << ">(";
        writeCString(os_, param_.key);
        os_ << ")";
        break;

      case CONTEXT:
        os_ << "context_";
        break;

      default:
        assert("invalid parameter type" == nullptr);
        break;
    }
  }
}

void Function::generateInvokerArguments(
    std::ostream& os_,
    int indent_) const {
  bool first_(true);
  for(const auto& param_ : parameters) {
    if(!first_)
      os_ << ",";
    else
      first_ = false;
    os_ << "\n";
    Formatting::printIndent(os_, indent_);
    os_ << param_.name;
  }
}

std::string Function::getName() const noexcept {
  return name;
}

std::string Function::getInvokerName() const noexcept {
  return invokerName(name);
}

std::string Function::getInvokerClass() const noexcept {
  return invokerClass(name);
}

void Function::addUserDataParameter(
    const std::string& name_,
    const std::string& key_,
    const std::string& type_) {
  assert(!name_.empty() && !key_.empty() && !type_.empty());
  parameters.push_back({USER_DATUM, name_, key_, type_});
}

void Function::addContextParameter(
    const std::string& name_) {
  assert(!name_.empty());
  parameters.push_back({CONTEXT, name_, "", ""});
}

void Function::generateMarshaler(
    std::ostream& os_,
    int indent_,
    const std::string& classname_) const {
  const std::string marshaler_name_(marshalerName(access, instance, clash, name));

  os_ << "class " << marshaler_name_ << " : public ::OTest2::FceMarshaler {\n";

  /* -- variables */
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "private:\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << classname_ << "* object;\n\n";

  Formatting::printIndent(os_, indent_ + 1);
  os_ << "public:\n";

  /* -- constructor */
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "explicit " << marshaler_name_ << "(\n";
  Formatting::printIndent(os_, indent_ + 4);
  os_ << classname_ << "* object_) :\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "object(object_) {\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "}\n\n";

  /* -- destructor */
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "virtual ~" << marshaler_name_ << "() {\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "}\n\n";

  /* -- marshaler function */
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "virtual void runFunction(\n";
  Formatting::printIndent(os_, indent_ + 4);
  os_ << "const ::OTest2::Context& context_) {\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "object->";
  if(access != FunctionAccess::NONE) {
    os_ << instance;
    switch(access) {
      case FunctionAccess::DOT:
        os_ << ".";
        break;
      case FunctionAccess::ARROW:
        os_ << "->";
        break;
      default:
        assert("invalid function access specification" == nullptr);
        break;
    }
  }
  os_ << name << "(";
  generateFceArguments(os_, indent_ + 5);
  os_ << ");\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "}\n";

  Formatting::printIndent(os_, indent_);
  os_ << "};";
}

void Function::generateInvocation(
    std::ostream& os_,
    int indent_,
    const std::string& fce_name_) const {
  Formatting::printIndent(os_, indent_);
  os_ << fce_name_ << "(";
  generateFceArguments(os_, indent_ + 2);
  os_ << ");";
}

void Function::generateDeclaration(
    std::ostream& os_,
    int indent_,
    const std::string& fce_name_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "void " << fce_name_ << "(";
  generateFceParameters(os_, indent_ + 2);
  os_ << ")";
}

void Function::generateRegistration(
    std::ostream& os_,
    int indent_,
    const std::string& classname_) const {
  const std::string marshaler_name_(marshalerName(access, instance, clash, name));
  os_ << "std::make_shared<" << marshaler_name_ << ">(this)";
}

void Function::generateInvoker(
    std::ostream& os_,
    int indent_,
    const std::string& classname_) const {
  const std::string invoker_class_(invokerClass(name));
  const std::string invoker_name_(invokerName(name));

  Formatting::printIndent(os_, indent_);
  os_ << "class " << invoker_class_ << " {\n";

  /* -- pointer to the owner object */
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "private:\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << classname_ << "* object;\n";

  os_ << "\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "public:\n";

  /* -- constructor */
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "explicit " << invoker_class_ << "(\n";
  Formatting::printIndent(os_, indent_ + 4);
  os_ << classname_ << "* object_) :\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "object(object_) {\n";
  os_ << "\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "}\n";

  /* -- invocation operator */
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "typename ::OTest2::TypeOfMine<" << rettype << ">::Type operator()(";
  generateFceParameters(os_, indent_ + 4);
  os_ << ") const {\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "return object->" << name << "(";
  generateInvokerArguments(os_, indent_ + 4);
  os_ << ");\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "}\n";

  Formatting::printIndent(os_, indent_);
  os_ << "} const " << invoker_name_ << ";";
}

} /* -- namespace OTest2 */

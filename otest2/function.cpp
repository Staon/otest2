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
#include <string>

#include "formatting.h"
#include "generfmt.h"
#include "vartable.h"

namespace OTest2 {

namespace {

std::string marshalerName(
    const std::string& fce_name_) {
  return "Marshaler_" + fce_name_;
}

} /* -- namespace */

Function::Function(
    const std::string& name_) :
  name(name_) {
  assert(!name.empty());

}

Function::~Function() {

}

void Function::addUserDataParameter(
    const std::string& name_,
    const std::string& key_,
    const std::string& type_) {
  assert(!name_.empty() && !key_.empty() && !type_.empty());
  parameters.push_back({name_, key_, type_});
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
    os_ << param_.type;
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
    os_ << "context_.user_data->getDatum<" << param_.type << ">(";
    writeCString(os_, param_.key);
    os_ << ")";
  }
}

void Function::generateMarshaler(
    std::ostream& os_,
    int indent_,
    const std::string& classname_) const {
  const std::string marshaler_name_(marshalerName(name));

  os_ << "class " << marshaler_name_ << " : public ::OTest2::FceMarshaler {\n";

  /* -- variables */
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "private:\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << classname_ << "* object;\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "void (" << classname_ << "::* fce)(";
  generateFceParameters(os_, indent_ + 4);
  os_ << ");\n\n";

  Formatting::printIndent(os_, indent_ + 1);
  os_ << "public:\n";

  /* -- constructor */
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "explicit " << marshaler_name_ << "(\n";
  Formatting::printIndent(os_, indent_ + 4);
  os_ << classname_ << "* object_,\n";
  Formatting::printIndent(os_, indent_ + 4);
  os_ << "void (" << classname_ << "::* fce_)(";
  generateFceParameters(os_, indent_ + 6);
  os_ << ")) :\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "object(object_),\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "fce(fce_) {\n";
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
  os_ << "(object->*fce)(";
  generateFceArguments(os_, indent_ + 5);
  os_ << ");\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << "}\n";

  Formatting::printIndent(os_, indent_);
  os_ << "};";
}

void Function::generateRegistration(
    std::ostream& os_,
    int indent_,
    const std::string& classname_) const {
  const std::string marshaler_name_(marshalerName(name));
  os_ << "std::make_shared<" << marshaler_name_ << ">(this, &"
      << classname_ << "::" << name << ")";
}

void Function::enrichVarTable(
    VarTable& table_) const {
  for(const auto& param_ : parameters)
    table_.appendUserData(param_.name, param_.key, param_.type);
}

} /* -- namespace OTest2 */

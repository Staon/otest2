/*
 * Copyright (C) 2021 Ondrej Starek
 *
 * This file is part of OTest2
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
#include "objectlist.h"

#include <assert.h>
#include <iostream>
#include <memory>
#include <otest2/utils.h>
#include <vector>

#include "formatting.h"
#include "generfmt.h"
#include "sectiontree.h"

namespace OTest2 {

namespace Parser {

namespace {

class ObjectRecord {
  public:
    /* -- avoid copying */
    ObjectRecord(
        const ObjectRecord&) = delete;
    ObjectRecord& operator = (
        const ObjectRecord&) = delete;

    ObjectRecord() = default;
    virtual ~ObjectRecord() = default;

    virtual void setRepeaterType(
        const std::string& repeater_type_) = 0;
    virtual std::string pushSection(
        const std::string& name_) = 0;
    virtual void popSection() = 0;
    virtual void printRegistrationInSuite(
        std::ostream& os_,
        const std::string& suite_,
        int indent_) const = 0;
    virtual void printRegistrationInFile(
        std::ostream& os_,
        const std::string& domain_,
        int indent_) const = 0;
};

typedef std::unique_ptr<ObjectRecord> ObjectRecordPtr;

class SuiteRecord : public ObjectRecord {
  private:
    std::string name;
    std::string repeater_type;
    ObjectTags tags;

  public:
    /* -- avoid copying */
    SuiteRecord(
        const SuiteRecord&) = delete;
    SuiteRecord& operator = (
        const SuiteRecord&) = delete;

    explicit SuiteRecord(
        const std::string& name_,
        const ObjectTags& tags_);
    virtual ~SuiteRecord() = default;
    virtual void setRepeaterType(
        const std::string& repeater_type_) override;
    virtual std::string pushSection(
        const std::string& name_) override;
    virtual void popSection() override;
    virtual void printRegistrationInSuite(
        std::ostream& os_,
        const std::string& suite_,
        int indent_) const override;
    virtual void printRegistrationInFile(
        std::ostream& os_,
        const std::string& domain_,
        int indent_) const override;
};

SuiteRecord::SuiteRecord(
    const std::string& name_,
    const ObjectTags& tags_) :
  name(name_),
  repeater_type(),
  tags(tags_) {

}

void SuiteRecord::setRepeaterType(
    const std::string& repeater_type_) {
  repeater_type = repeater_type_;
}

std::string SuiteRecord::pushSection(
    const std::string& name_) {
  assert(false);
  return std::string();
}

void SuiteRecord::popSection() {
  assert(false);
}

void SuiteRecord::printRegistrationInSuite(
    std::ostream& os_,
    const std::string& suite_,
    int indent_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "{\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "auto scenario_(std::make_shared< ::OTest2::ScenarioSuite >(\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "\"" << name << "\",\n";
  Formatting::printIndent(os_, indent_ + 3);
  writeTags(os_, tags, indent_ + 2);
  os_ << ",\n";
  Formatting::printIndent(os_, indent_ + 3);
  if(repeater_type.empty())
    os_ << "std::make_shared< ::OTest2::ObjectRepeaterFactoryOnceNested<" << suite_ << ", " << name << "> >(\n";
  else
    os_ << "std::make_shared< ::OTest2::ObjectRepeaterFactoryMultiNested<" << suite_ << ", " << name << ", " << repeater_type << " > >(\n";
  Formatting::printIndent(os_, indent_ + 5);
  os_ << "&" << suite_ << "::createSuite_" << name << ")));\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ << name << "::registerAllChildren(scenario_);\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "parent_->appendScenario(scenario_);\n";
  Formatting::printIndent(os_, indent_);
  os_ << "}\n";
}

void SuiteRecord::printRegistrationInFile(
    std::ostream& os_,
    const std::string& domain_,
    int indent_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "{\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "auto scenario_(std::make_shared< ::OTest2::ScenarioSuite >(\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "\"" << name << "\",\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "";
  writeTags(os_, tags, indent_ + 3);
  os_ << ",\n";
  Formatting::printIndent(os_, indent_ + 3);
  if(repeater_type.empty())
    os_ << "std::make_shared< ::OTest2::ObjectRepeaterFactoryOnceRoot< " << name;
  else
    os_ << "std::make_shared< ::OTest2::ObjectRepeaterFactoryMultiRoot< " << name << ", " << repeater_type;
  os_<< " > >()));\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_<< name << "::registerAllChildren(scenario_);\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ <<"::OTest2::Registry::instance(";
  writeCString(os_, domain_);
  os_ << ").registerScenario(scenario_);\n";
  Formatting::printIndent(os_, indent_);
  os_ << "}\n";
}

class CaseRecord : public ObjectRecord {
  private:
    std::string name;
    std::string repeater_type;
    SectionTree sections;
    ObjectTags tags;

  public:
    /* -- avoid copying */
    CaseRecord(
        const CaseRecord&) = delete;
    CaseRecord& operator = (
        const CaseRecord&) = delete;

    explicit CaseRecord(
        const std::string& name_,
        const ObjectTags& tags_);
    virtual ~CaseRecord() = default;
    virtual void setRepeaterType(
        const std::string& repeater_type_) override;
    virtual std::string pushSection(
        const std::string& name_) override;
    virtual void popSection() override;
    virtual void printRegistrationInSuite(
        std::ostream& os_,
        const std::string& suite_,
        int indent_) const override;
    virtual void printRegistrationInFile(
        std::ostream& os_,
        const std::string& domain_,
        int indent_) const override;
};

CaseRecord::CaseRecord(
    const std::string& name_,
    const ObjectTags& tags_) :
  name(name_),
  repeater_type(),
  tags(tags_) {

}

void CaseRecord::setRepeaterType(
    const std::string& repeater_type_) {
  repeater_type = repeater_type_;
}

std::string CaseRecord::pushSection(
    const std::string& name_) {
  sections.pushSection(name_);
  return sections.getSectionPath();
}

void CaseRecord::popSection() {
  sections.popSection();
}

void CaseRecord::printRegistrationInSuite(
    std::ostream& os_,
    const std::string& suite_,
    int indent_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "{\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "auto scenario_(::OTest2::ScenarioCase::createBuilder(\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "\"" << name << "\",\n";
  Formatting::printIndent(os_, indent_ + 3);
  writeTags(os_, tags, indent_ + 3);
  os_ << ",\n";
  Formatting::printIndent(os_, indent_ + 3);
  if(repeater_type.empty())
    os_ << "std::make_shared< ::OTest2::ObjectRepeaterFactoryOnceNested<" << suite_ << ", " << name << "> >(\n";
  else
    os_ << "std::make_shared< ::OTest2::ObjectRepeaterFactoryMultiNested<" << suite_ << ", " << name << ", " << repeater_type << " > >(\n";
  Formatting::printIndent(os_, indent_ + 5);
  os_ << "&" << suite_ << "::createCase_" << name << "))";
  sections.printRegistration(os_, indent_ + 2);
  os_ << "\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << ".getScenario());\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "parent_->appendScenario(scenario_);\n";
  Formatting::printIndent(os_, indent_);
  os_ << "}\n";
}

void CaseRecord::printRegistrationInFile(
    std::ostream& os_,
    const std::string& domain_,
    int indent_) const {
  Formatting::printIndent(os_, indent_);
  os_ << "{\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ << "auto scenario_(::OTest2::ScenarioCase::createBuilder(\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "\"" << name << "\",\n";
  Formatting::printIndent(os_, indent_ + 3);
  os_ << "";
  writeTags(os_, tags, indent_ + 3);
  os_ << ",\n";
  Formatting::printIndent(os_, indent_ + 3);
  if(repeater_type.empty())
    os_ << "std::make_shared< ::OTest2::ObjectRepeaterFactoryOnceRoot< " << name;
  else
    os_ << "std::make_shared< ::OTest2::ObjectRepeaterFactoryMultiRoot< " << name << ", " << repeater_type;
  os_<< " > >())";
  sections.printRegistration(os_, indent_ + 2);
  os_ << "\n";
  Formatting::printIndent(os_, indent_ + 2);
  os_ << ".getScenario());\n";
  Formatting::printIndent(os_, indent_ + 1);
  os_ <<"::OTest2::Registry::instance(";
  writeCString(os_, domain_);
  os_ << ").registerScenario(scenario_);\n";
  Formatting::printIndent(os_, indent_);
  os_ << "}\n";
}

} /* -- namespace */

struct ObjectList::Impl {
    std::vector<ObjectRecordPtr> objects;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    Impl();
    ~Impl();
};

ObjectList::Impl::Impl() {

}

ObjectList::Impl::~Impl() {

}

ObjectList::ObjectList() :
  pimpl(new Impl) {

}

ObjectList::~ObjectList() {
  odelete(pimpl);
}

void ObjectList::appendSuite(
    const std::string& name_,
    const ObjectTags& tags_) {
  assert(!name_.empty());
  pimpl->objects.push_back(
      ::OTest2::make_unique<SuiteRecord>(name_, tags_));
}

void ObjectList::appendCase(
    const std::string& name_,
    const ObjectTags& tags_) {
  assert(!name_.empty());
  pimpl->objects.push_back(
      ::OTest2::make_unique<CaseRecord>(name_, tags_));
}

bool ObjectList::isEmpty() const noexcept {
  return pimpl->objects.empty();
}

void ObjectList::setRepeaterType(
    const std::string& repeater_type_) {
  assert(!pimpl->objects.empty());
  pimpl->objects.back()->setRepeaterType(repeater_type_);
}

std::string ObjectList::pushSection(
    const std::string& name_) {
  assert(!pimpl->objects.empty());
  return pimpl->objects.back()->pushSection(name_);
}

void ObjectList::popSection() {
  assert(!pimpl->objects.empty());
  pimpl->objects.back()->popSection();
}

void ObjectList::printRegistrationsInSuite(
    std::ostream& os_,
    const std::string& suite_,
    int indent_) {
  for(const auto& object : pimpl->objects)
    object->printRegistrationInSuite(os_, suite_, indent_);
}

void ObjectList::printRegistrationsInFile(
    std::ostream& os_,
    const std::string& domain_,
    int indent_) {
  for(const auto& object : pimpl->objects)
    object->printRegistrationInFile(os_, domain_, indent_);
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

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
#include "sectiontree.h"

#include <assert.h>
#include <iostream>
#include <sstream>

#include "formatting.h"
#include "generfmt.h"

namespace OTest2 {

namespace Parser {

SectionTree::SectionTree() = default;
SectionTree::~SectionTree() = default;

bool SectionTree::empty() const noexcept {
  return root_sections.empty();
}

void SectionTree::clear() noexcept {
  root_sections.clear();
}

void SectionTree::pushSection(
    const std::string& name_) {
  assert(!name_.empty());

  if(section_stack.empty()) {
    root_sections.push_back(Section{name_, {}});
    section_stack.push_back(&root_sections.back());
  }
  else {
    auto* top_(section_stack.back());
    top_->children.push_back(Section{name_, {}});
    section_stack.push_back(&top_->children.back());
  }
}

void SectionTree::popSection() {
  assert(!section_stack.empty());
  section_stack.pop_back();
}

std::string SectionTree::getSectionPath() const {
  std::ostringstream oss_;
  bool first_(true);
  for(const auto* section_ : section_stack) {
    if(first_)
      first_ = false;
    else
      oss_ << "::";
    oss_ << section_->name;
  }
  return oss_.str();
}

void SectionTree::printRegistrationImpl(
    std::ostream& os_,
    const Section& section_,
    int indent_) const {
  Formatting::printIndent(os_, indent_);
  os_ << ".pushSection(";
  writeCString(os_, section_.name);
  os_ << ")";

  /* -- register nested sections */
  for(const auto& subsection_ : section_.children) {
    os_ << "\n";
    printRegistrationImpl(os_, subsection_, indent_ + 1);
  }

  os_ << "\n";
  Formatting::printIndent(os_, indent_);
  os_ << ".popSection()";
}

void SectionTree::printRegistration(
    std::ostream& os_,
    int indent_) const {
  for(const auto& section_ : root_sections) {
    os_ << "\n";
    printRegistrationImpl(os_, section_, indent_);
  }
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

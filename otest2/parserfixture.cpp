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
#include "parserfixture.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Type.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "generator.h"
#include "parserannotationimpl.h"
#include "parsercontextimpl.h"
#include "parserfunction.h"
#include "parsertype.h"

namespace OTest2 {

namespace Parser {

namespace {

const std::string REPEATER_CLASS("OTest2::Repeater");

struct FixtureClassification {
    std::string varname;
    std::string type;

    /* -- variable initializer */
    bool initializer;
    clang::SourceRange init_range;

    /* -- variable is a user datum fixture */
    bool user_data;
    std::string user_data_key;

    /* -- variable is a repeater */
    bool repeater;

    /* -- class access - if the fixture is a class, a reference to a class,
     *    a pointer to a class or a smart-pointer to a class, this variable
     *    says how to access the fixture methods. */
    FunctionAccess fixture_access;
    struct Fixture {
        FunctionPtr start_up_fce;
        FunctionPtr tear_down_fce;
    };
    std::vector<Fixture> fixtures;

    FixtureClassification();
};

FixtureClassification::FixtureClassification() :
  varname(),
  type(),
  initializer(false),
  init_range(),
  user_data(false),
  user_data_key(),
  repeater(false),
  fixture_access(FunctionAccess::NONE),
  fixtures() {

}

bool parseVariableInitializer(
    ParserContext* context_,
    FixtureClassification& classification_,
    clang::VarDecl* vardecl_) {
  /* -- Get the initializer. Default-initialized class is handled as
   *    CxxConstructExpr without parameters. */
  clang::Expr* init_(vardecl_->getInit());
  if(init_ == nullptr)
    return true; /* -- no error */

  /* -- just the C++ call initializer is supported */
  if(vardecl_->getInitStyle() != clang::VarDecl::CallInit) {
    context_->setError("only the callinit (C++98) initializer is supported", vardecl_);
    return false;
  }

  /* -- the variable is initialized */
  classification_.initializer = true;

  /* -- an expression with cleanups is a wrapper, I nest into it. */
  if(clang::isa<clang::ExprWithCleanups>(init_)) {
    init_ = clang::cast<clang::ExprWithCleanups>(init_)->getSubExpr();
  }

//  std::cout << init_->getStmtClassName() << std::endl;

  /* -- adjust the range for constructor call */
  if(clang::isa<clang::CXXConstructExpr>(init_)) {
    clang::CXXConstructExpr* ctrexpr_(
        clang::cast<clang::CXXConstructExpr>(init_));
    const int argnum_(ctrexpr_->getNumArgs());

    /* -- find first and last non-default argument */
    int last_index_(-1);
    if(argnum_ > 0) {
      for(int i_(0); i_ < argnum_; ++i_) {
        clang::Expr *arg_(ctrexpr_->getArg(i_));
        if(clang::isa<clang::CXXDefaultArgExpr>(arg_))
          break;
        last_index_ = i_;
      }
    }

    if(last_index_ < 0) {
      /* -- There are no parameters or all of them are defaulted. Handle that
       *    as there is no initializer. */
      classification_.initializer = false;
      return true;
    }
    else {
      /* -- get source range of the arguments */
      clang::SourceRange first_(context_->getNodeRange(ctrexpr_->getArg(0)));
      clang::SourceRange last_(
          context_->getNodeRange(ctrexpr_->getArg(last_index_)));
      classification_.init_range = clang::SourceRange(first_.getBegin(), last_.getEnd());
      return true;
    }
  }

  /* -- something different than a constructor call */
  classification_.init_range = context_->getNodeRange(init_);
  return true;
}

bool parseUserData(
    ParserContext* context_,
    FixtureClassification& classification_,
    clang::VarDecl* vardecl_) {
  AnnotationRegex annotation_(USER_DATA_VAR_ANNOTATION);
  classification_.user_data = hasAnnotation(vardecl_, annotation_);
  if(classification_.user_data) {
    classification_.user_data_key = annotation_.matches[1].str();
    if(classification_.user_data_key.empty())
      classification_.user_data_key = classification_.varname;
  }
  return true;
}

bool parseClass(
    ParserContext* context_,
    FixtureClassification& classification_,
    const clang::CXXRecordDecl* object_,
    clang::VarDecl* vardecl_) {
  int counter_(0);
  return !traverseClasses(context_, object_, true, [&](const clang::CXXRecordDecl* klass_) {
    clang::QualType klass_type_(context_->comp_context->getRecordType(klass_));
    std::string klass_name_(parseType(context_, klass_type_));
//    std::cout << "class type: " << klass_name_ << std::endl;
    if(klass_name_ == REPEATER_CLASS) {
      classification_.repeater = true;
    }

    /* -- One fixture object may have several fixture methods because the
     *    inheritance. Following string is used to resolve collisions
     *    in the names of marshaler. */
    ++counter_;
    std::ostringstream clash_;
    clash_ << counter_;

    /* -- find the fixture methods */
    FunctionPtr start_up_;
    FunctionPtr tear_down_;
    for(auto* method_ : klass_->methods()) {
      if(method_->isInstance()
          && !method_->isDeleted()
          && method_->getDeclKind() == clang::Decl::Kind::CXXMethod
          && !method_->isOverloadedOperator()) {
        if(hasAnnotation(method_, START_UP_ANNOTATION)) {
          if(start_up_ != nullptr) {
            context_->setError(
                "duplicate start-up method in the class '" + klass_->getNameAsString() + "'",
                vardecl_);
            return true;
          }

          start_up_ = createFunctionObject(
              context_,
              classification_.fixture_access,
              classification_.varname,
              clash_.str(),
              method_);
        }
        else if(hasAnnotation(method_, TEAR_DOWN_ANNOTATION)) {
          if(tear_down_ != nullptr) {
            context_->setError(
                "duplicate tear-down method in the class '" + klass_->getNameAsString() + "'",
                vardecl_);
            return true;
          }

          tear_down_ = createFunctionObject(
              context_,
              classification_.fixture_access,
              classification_.varname,
              clash_.str(),
              method_);
        }
      }
    }

    /* -- create the fixture record */
    if(start_up_ != nullptr || tear_down_ != nullptr)
      classification_.fixtures.push_back({start_up_, tear_down_});

    return false;
  });
}

bool parseSmartPointer(
    ParserContext* context_,
    FixtureClassification& classification_,
    const clang::CXXRecordDecl* object_,
    clang::VarDecl* vardecl_) {
  /* -- find the arrow operator */
  const clang::CXXMethodDecl* operator_(nullptr);
  if(traverseMethods(context_, object_, false, [&operator_](const clang::CXXMethodDecl* method_) {
    auto operator_kind_(method_->getOverloadedOperator());
    if(operator_kind_ == clang::OO_Arrow && !method_->isDeleted()) {
      operator_ = method_;
      return true;
    }
    return false;
  })) {
    /* -- The object has overloaded the arrow operator. Get the type
     *    which is pointed. */
    const clang::Type* result_type_(operator_->getCallResultType().getTypePtr());
    const clang::CXXRecordDecl* pointee_(result_type_->getAsCXXRecordDecl());
    if(pointee_ == nullptr)
      pointee_ = result_type_->getPointeeCXXRecordDecl();

    /* -- the smart pointer doesn't point a C++ class -> handle it as a regular
     *    fixture variable. */
    if(pointee_ == nullptr) {
      classification_.fixture_access = FunctionAccess::NONE;
      return true;
    }

    /* -- C++ compilers apply the arrow operator recursively. So do we. */
    classification_.fixture_access = FunctionAccess::ARROW;
    return parseSmartPointer(context_, classification_, pointee_, vardecl_);
  }

  /* -- The class is not a smart pointer. Parse just the class. */
  return parseClass(context_, classification_, object_, vardecl_);
}

bool parseFixtureAccess(
    ParserContext* context_,
    FixtureClassification& classification_,
    clang::VarDecl* vardecl_) {
  const clang::Type* rawtype_(vardecl_->getType().getTypePtr());

  /* -- direct class instance */
  const clang::CXXRecordDecl* object_(rawtype_->getAsCXXRecordDecl());
  if(object_ != nullptr) {
    classification_.fixture_access = FunctionAccess::DOT;
    return parseSmartPointer(context_, classification_, object_, vardecl_);
  }

  /* -- a reference or a pointer to a class */
  object_ = rawtype_->getPointeeCXXRecordDecl();
  if(object_ != nullptr) {
    if(rawtype_->isReferenceType()) {
      /* -- I want to allow references to smart pointers. */
      classification_.fixture_access = FunctionAccess::DOT;
      return parseSmartPointer(context_, classification_, object_, vardecl_);
    }
    else if(rawtype_->isPointerType()) {
      classification_.fixture_access = FunctionAccess::ARROW;
      return parseClass(context_, classification_, object_, vardecl_);
    }
    else {
      context_->setError("some strange reference type of the fixture", vardecl_);
      return false;
    }
  }

  /* -- all other variables are handled as ordinary fixture variable */
  return true;
}

bool classifyFixture(
    ParserContext* context_,
    FixtureClassification& classification_,
    clang::VarDecl* vardecl_) {
  /* -- get variable name and type */
  classification_.varname = vardecl_->getNameAsString();
  classification_.type = parseType(context_, vardecl_->getType());

  /* -- classify type of the fixture */
  if(!parseVariableInitializer(context_, classification_, vardecl_))
    return false;
  if(!parseUserData(context_, classification_, vardecl_))
    return false;
  if(!parseFixtureAccess(context_, classification_, vardecl_))
    return false;
  return true;
}

} /* -- namespace */

bool parseVariable(
    ParserContext* context_,
    clang::VarDecl* vardecl_) {
  /* -- get kind of the fixture */
  FixtureClassification classification_;
  if(!classifyFixture(context_, classification_, vardecl_))
    return false;

  /* -- generate user data */
  if(classification_.user_data) {
    /* -- the initialization of user data is generated. There must not be
     *    a custom initializer here. */
    if(classification_.initializer) {
      context_->setError("the user data variable must not be initialized", vardecl_);
      return false;
    }

    context_->generator->appendUserData(
        classification_.varname,
        classification_.user_data_key,
        classification_.type);
    return true;
  }

  /* -- A fixture object - register invocation of the start-up and tear-down
   *    methods. */
  if(!classification_.fixtures.empty()) {
    for(const auto& fixture_ : classification_.fixtures) {
      context_->generator->appendFixtureObject(
          classification_.varname,
          fixture_.start_up_fce,
          fixture_.tear_down_fce);
    }
  }

  /* -- A repeater object. The repeater object must be accessed by the
   *    dot operator because the object is published into the testing object
   *    as a reference.
   *
   *    Note: the repeater object may be a fixture object too! */
  if(classification_.repeater) {
    if(classification_.fixture_access != FunctionAccess::DOT) {
      context_->setError("the repeater object must not be any pointer", vardecl_);
      return false;
    }

    /* -- generate the repeater */
    bool added_repeater_(false);
    if(classification_.initializer) {
      added_repeater_ = context_->generator->appendRepeaterInit(
          classification_.varname,
          classification_.type,
          context_->createLocation(classification_.init_range.getBegin()),
          context_->createLocation(classification_.init_range.getEnd()));
    }
    else {
      added_repeater_ = context_->generator->appendRepeater(
          classification_.varname,
          classification_.type);
    }
    if(!added_repeater_) {
      context_->setError(
          "just one repeater object may be declared in one testing object",
          vardecl_);
      return false;
    }

    return true;
  }

  /* -- ordinary fixture variable */
  if(classification_.initializer) {
    context_->generator->appendVariableInit(
        classification_.varname,
        classification_.type,
        context_->createLocation(classification_.init_range.getBegin()),
        context_->createLocation(classification_.init_range.getEnd()));
  }
  else {
    context_->generator->appendVariable(
        classification_.varname, classification_.type);
  }
  return true;
}

} /* -- namespace Parser */

} /* -- namespace OTest2 */

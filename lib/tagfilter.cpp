/*
 * Copyright (C) 2020 Ondrej Starek
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
#include <tagfilter.h>

#include <assert.h>
#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

#include <tagsstack.h>
#include <utils.h>

namespace OTest2 {

TagExpressionException::TagExpressionException(
    const std::string& message_) :
  message(message_) {

}

TagExpressionException::TagExpressionException(
    int location_,
    const std::string& token_) {
  std::ostringstream oss_;
  oss_ << "syntax error at " << location_ << ": unexpected token '" << token_ << "'";
  message = oss_.str();
}

TagExpressionException::TagExpressionException(
    int location_,
    char c_) {
  std::ostringstream oss_;
  oss_ << "invalid character '" << c_ << "' at location " << location_;
  message = oss_.str();
}

TagExpressionException::TagExpressionException(
    TagExpressionException&& exc_) :
  Exception(std::move(exc_)),
  message(std::move(exc_.message)) {

}

std::string TagExpressionException::reason() const {
  return message;
}

namespace {

const std::string EMPTY_CLASS("empty");

class TagExpression {
  public:
    TagExpression() = default;
    virtual ~TagExpression() = default;

    /* -- avoid copying */
    TagExpression(
        const TagExpression&) = delete;
    TagExpression& operator = (
        const TagExpression&) = delete;

    /**
     * @brief Return true if the tags matches the expression
     */
    virtual bool matches(
        bool parent_,
        const TagsStack& tags_stack_) const noexcept = 0;
};

typedef std::shared_ptr<TagExpression> TagExpressionPtr;

class TagExpressionOr : public TagExpression {
  private:
    TagExpressionPtr left;
    TagExpressionPtr right;

  public:
    explicit TagExpressionOr(
        TagExpressionPtr left_,
        TagExpressionPtr right_);
    virtual ~TagExpressionOr() = default;

    /* -- avoid copying */
    TagExpressionOr(
        const TagExpressionOr&) = delete;
    TagExpressionOr& operator = (
        const TagExpressionOr&) = delete;

    virtual bool matches(
        bool parent_,
        const TagsStack& tags_stack_) const noexcept override;
};

class TagExpressionAnd : public TagExpression {
  private:
    TagExpressionPtr left;
    TagExpressionPtr right;

  public:
    explicit TagExpressionAnd(
        TagExpressionPtr left_,
        TagExpressionPtr right_);
    virtual ~TagExpressionAnd() = default;

    /* -- avoid copying */
    TagExpressionAnd(
        const TagExpressionAnd&) = delete;
    TagExpressionAnd& operator = (
        const TagExpressionAnd&) = delete;

    virtual bool matches(
        bool parent_,
        const TagsStack& tags_stack_) const noexcept override;
};

class TagExpressionTag : public TagExpression {
  private:
    std::string tag;

  public:
    explicit TagExpressionTag(
        const std::string& tag_);
    virtual ~TagExpressionTag() = default;

    /* -- avoid copying */
    TagExpressionTag(
        const TagExpressionTag&) = delete;
    TagExpressionTag& operator = (
        const TagExpressionTag&) = delete;

    virtual bool matches(
        bool parent_,
        const TagsStack& tags_stack_) const noexcept override;
};

class TagExpressionEmpty : public TagExpression {
  public:
    TagExpressionEmpty() = default;
    virtual ~TagExpressionEmpty() = default;

    /* -- avoid copying */
    TagExpressionEmpty(
        const TagExpressionEmpty&) = delete;
    TagExpressionEmpty& operator = (
        const TagExpressionEmpty&) = delete;

    virtual bool matches(
        bool parent_,
        const TagsStack& tags_stack_) const noexcept override;
};

class TagExpressionNot : public TagExpression {
  private:
    TagExpressionPtr operand;

  public:
    explicit TagExpressionNot(
        TagExpressionPtr operand_);
    virtual ~TagExpressionNot() = default;

    /* -- avoid copying */
    TagExpressionNot(
        const TagExpressionNot&) = delete;
    TagExpressionNot& operator = (
        const TagExpressionNot&) = delete;

    virtual bool matches(
        bool parent_,
        const TagsStack& tags_stack_) const noexcept override;
};

class TagExpressionParent : public TagExpression {
  private:
    TagExpressionPtr operand;

  public:
    explicit TagExpressionParent(
        TagExpressionPtr operand_);
    virtual ~TagExpressionParent() = default;

    /* -- avoid copying */
    TagExpressionParent(
        const TagExpressionParent&) = delete;
    TagExpressionParent& operator = (
        const TagExpressionParent&) = delete;

    virtual bool matches(
        bool parent_,
        const TagsStack& tags_stack_) const noexcept override;
};

class TagExpressionNull : public TagExpression {
  public:
    TagExpressionNull() = default;
    virtual ~TagExpressionNull() = default;

    /* -- avoid copying */
    TagExpressionNull(
        const TagExpressionNull&) = delete;
    TagExpressionNull& operator = (
        const TagExpressionNull&) = delete;

    virtual bool matches(
        bool parent_,
        const TagsStack& tags_stack_) const noexcept override;
};

TagExpressionOr::TagExpressionOr(
    TagExpressionPtr left_,
    TagExpressionPtr right_) :
  left(left_),
  right(right_) {

}

bool TagExpressionOr::matches(
    bool parent_,
    const TagsStack& tags_stack_) const noexcept {
  return left->matches(parent_, tags_stack_) || right->matches(parent_, tags_stack_);
}

TagExpressionAnd::TagExpressionAnd(
    TagExpressionPtr left_,
    TagExpressionPtr right_) :
  left(left_),
  right(right_) {

}

bool TagExpressionAnd::matches(
    bool parent_,
    const TagsStack& tags_stack_) const noexcept {
  return left->matches(parent_, tags_stack_) && right->matches(parent_, tags_stack_);
}

TagExpressionTag::TagExpressionTag(
    const std::string& tag_) :
  tag(tag_) {

}

bool TagExpressionTag::matches(
    bool parent_,
    const TagsStack& tags_stack_) const noexcept {
  if(!parent_) {
    if(tags_stack_.isTopSuite())
      return true; /* -- enter the suite even though it doesn't contain the tag */
    else
      return tags_stack_.findTag(tag);
  }
  else {
    return tags_stack_.findTagParent(tag);
  }
}

bool TagExpressionEmpty::matches(
    bool parent_,
    const TagsStack& tags_stack_) const noexcept {
  if(!parent_) {
    if(tags_stack_.isTopEmpty())
      return true; /* -- always entry the suite */
    else
      return tags_stack_.isTopEmpty();
  }
  else {
    return tags_stack_.allAreEmpty();
  }
}

TagExpressionNot::TagExpressionNot(
    TagExpressionPtr operand_) :
  operand(operand_) {

}

bool TagExpressionNot::matches(
    bool parent_,
    const TagsStack& tags_stack_) const noexcept {
  return !operand->matches(parent_, tags_stack_);
}

TagExpressionParent::TagExpressionParent(
    TagExpressionPtr operand_) :
  operand(operand_) {

}

bool TagExpressionParent::matches(
    bool parent_,
    const TagsStack& tags_stack_) const noexcept {
  return operand->matches(true, tags_stack_);
}

bool TagExpressionNull::matches(
    bool parent_,
    const TagsStack& tags_stack_) const noexcept {
  return true; /* -- always true */
}

enum class TokenType {
  NONE,
  TAG,   /**< a tag name */
  PARENT,/**< reference to a parent tag */
  CLASS, /**< a tag class name */
  NOT,   /**< not operator */
  AND,   /**< and operator */
  OR,    /**< or operator */
  LEFT,  /**< left parenthesis */
  RIGHT, /**< right parenthesis */
  END,   /**< end of expression */
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexan {
  private:
    enum class State {
      START,
      TAG,
      CLASS,
      OR,
      AND,
    };

    std::istream* is;
    State state;
    std::string value;
    int location;
    Token prepared;

    Token resetValue(
        TokenType type_);

  public:
    explicit Lexan(
        std::istream* is_);
    Token getNextToken();
    int getLocation() const;
};

Lexan::Lexan(
    std::istream* is_) :
  is(is_),
  state(State::START),
  value(),
  location(0),
  prepared{TokenType::NONE, ""} {

}

Token Lexan::resetValue(
    TokenType type_) {
  Token retval_{type_, value};
  value.clear();
  return retval_;
}

Token Lexan::getNextToken() {
  /* -- parsed token from previous run */
  if(prepared.type != TokenType::NONE) {
    Token retval_(prepared);
    prepared.type = TokenType::NONE;
    return retval_;
  }

  while(true) {
    int c_(is->get());
    ++location;

    switch(state) {
      case State::START:
        if(std::istream::traits_type::not_eof(c_)) {
          if(std::isspace(c_)) {
            /* -- ignore spaces */
          }
          else if(std::isalpha(c_) || c_ == '_') {
            state = State::TAG;
            value.push_back(c_);
          }
          else if(c_ == '^') {
            return {TokenType::PARENT, "^"};
          }
          else if(c_ == '<') {
            state = State::CLASS;
          }
          else if(c_ == '!') {
            return {TokenType::NOT, "!"};
          }
          else if(c_ == '|') {
            state = State::OR;
          }
          else if(c_ == '&') {
            state = State::AND;
          }
          else if(c_ == '(') {
            return {TokenType::LEFT, "("};
          }
          else if(c_ == ')') {
            return {TokenType::RIGHT, ")"};
          }
          else {
            /* -- invalid character */
            throw TagExpressionException(location, c_);
          }
          break;
        }
        else {
          /* -- end of the string -> OK here */
          return {TokenType::END, ""};
        }

      case State::TAG:
        if(std::istream::traits_type::not_eof(c_)) {
          if(std::isspace(c_)) {
            state = State::START;
            return resetValue(TokenType::TAG);
          }
          else if(std::isalnum(c_) || c_ == '_' || c_ == ':' || c_ == '-' || c_ == '.') {
            value.push_back(c_);
          }
          else if(c_ == '^') {
            prepared = {TokenType::PARENT, "^"};
            state = State::START;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == '<') {
            state = State::CLASS;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == '!') {
            prepared = {TokenType::NOT, "!"};
            state = State::START;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == '|') {
            state = State::OR;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == '&') {
            state = State::AND;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == '(') {
            prepared = {TokenType::LEFT, "("};
            state = State::START;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == ')') {
            prepared = {TokenType::RIGHT, ")"};
            state = State::START;
            return resetValue(TokenType::TAG);
          }
          else {
            throw TagExpressionException(location, c_);
          }
          break;
        }
        else {
          /* -- end of string -> return the parsed tag */
          state = State::START;
          return resetValue(TokenType::TAG);
        }

      case State::CLASS:
        if(std::istream::traits_type::not_eof(c_)) {
          if(std::isalpha(c_)) {
            value.push_back(c_);
          }
          else if(c_ == '>') {
            state = State::START;
            return resetValue(TokenType::CLASS);
          }
          else {
            throw TagExpressionException(location, c_);
          }
          break;
        }
        else {
          throw TagExpressionException("unexpected end of the expression");
        }

      case State::OR:
        if(std::istream::traits_type::not_eof(c_)) {
          if(c_ == '|') {
            state = State::START;
            return {TokenType::OR, "||"};
          }
          else {
            throw TagExpressionException(location, c_);
          }
        }
        else {
          throw TagExpressionException("unexpected end of the expression");
        }

      case State::AND:
        if(std::istream::traits_type::not_eof(c_)) {
          if(c_ == '&') {
            state = State::START;
            return {TokenType::AND, "&&"};
          }
          else {
            throw TagExpressionException(location, c_);
          }
        }
        else {
          throw TagExpressionException("unexpected end of the expression");
        }

      default:
        assert(false);
        break;
    }
  }
}

int Lexan::getLocation() const {
  return location;
}

class SyntaxParser {
  private:
    Lexan* lexan;
    Token peek;

    Token lookAhead();
    void compare(
        TokenType type_);

    TagExpressionPtr expression();
    TagExpressionPtr expressionTail(
        TagExpressionPtr left_term_);
    TagExpressionPtr term();
    TagExpressionPtr termTail(
        TagExpressionPtr left_factor_);
    TagExpressionPtr factor();
    TagExpressionPtr createClass(
        const std::string& class_name_);

  public:
    explicit SyntaxParser(
        Lexan* lexan_);

    TagExpressionPtr parse();
};

SyntaxParser::SyntaxParser(
    Lexan* lexan_) :
  lexan(lexan_),
  peek{TokenType::NONE, ""} {

}

Token SyntaxParser::lookAhead() {
  if(peek.type == TokenType::NONE) {
    peek = lexan->getNextToken();
  }
  return peek;
}

void SyntaxParser::compare(
    TokenType type_) {
  /* -- read the token */
  if(peek.type == TokenType::NONE) {
    peek = lexan->getNextToken();
  }

  /* -- check the token type */
  if(peek.type != type_) {
    throw TagExpressionException(lexan->getLocation(), peek.value);
  }

  /* -- consume the token */
  peek.type = TokenType::NONE;
}

TagExpressionPtr SyntaxParser::expression() {
  return expressionTail(term());
}

TagExpressionPtr SyntaxParser::expressionTail(
    TagExpressionPtr left_term_) {
  Token l_(lookAhead());
  if(l_.type == TokenType::OR) {
    compare(TokenType::OR);
    TagExpressionPtr right_(term());
    return expressionTail(std::make_shared<TagExpressionOr>(left_term_, right_));
  }
  else {
    return left_term_;
  }
}

TagExpressionPtr SyntaxParser::term() {
  return termTail(factor());
}

TagExpressionPtr SyntaxParser::termTail(
    TagExpressionPtr left_factor_) {
  Token l_(lookAhead());
  if(l_.type == TokenType::AND) {
    compare(TokenType::AND);
    TagExpressionPtr right_(factor());
    return termTail(std::make_shared<TagExpressionAnd>(left_factor_, right_));
  }
  else {
    return left_factor_;
  }
}

TagExpressionPtr SyntaxParser::factor() {
  Token l_(lookAhead());
  if(l_.type == TokenType::TAG) {
    compare(TokenType::TAG);
    return std::make_shared<TagExpressionTag>(l_.value);
  }
  else if(l_.type == TokenType::PARENT) {
    compare(TokenType::PARENT);
    return std::make_shared<TagExpressionParent>(factor());
  }
  else if(l_.type == TokenType::CLASS) {
    compare(TokenType::CLASS);
    return createClass(l_.value);
  }
  else if(l_.type == TokenType::NOT) {
    compare(TokenType::NOT);
    return std::make_shared<TagExpressionNot>(factor());
  }
  else if(l_.type == TokenType::LEFT) {
    compare(TokenType::LEFT);
    return expression();
    compare(TokenType::RIGHT);
  }
  else {
    throw TagExpressionException(lexan->getLocation(), l_.value);
  }
}

TagExpressionPtr SyntaxParser::createClass(
    const std::string& class_name_) {
  if(class_name_ == EMPTY_CLASS) {
    return std::make_shared<TagExpressionEmpty>();
  }
  else {
    throw TagExpressionException(lexan->getLocation(), class_name_);
  }
}

TagExpressionPtr SyntaxParser::parse() {
  Token l_(lookAhead());
  TagExpressionPtr expr_;
  if(l_.type == TokenType::END) {
    expr_ = std::make_shared<TagExpressionNull>();
  }
  else {
    expr_ = expression();
  }
  compare(TokenType::END);
  return expr_;
}

} /* -- namespace */

struct TagFilter::Impl {
    TagExpressionPtr expression;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& tag_expression_);
    ~Impl();
};

TagFilter::Impl::Impl(
    const std::string& tag_expression_) {
  std::istringstream iss_(tag_expression_);
  Lexan lexan_(&iss_);
  SyntaxParser parser_(&lexan_);
  expression = parser_.parse();
}

TagFilter::Impl::~Impl() {

}

TagFilter::TagFilter() :
  pimpl(new Impl("")) {

}

TagFilter::TagFilter(
    const std::string& tag_expression_) :
  pimpl(new Impl(tag_expression_)) {

}

TagFilter::~TagFilter() {
  odelete(pimpl);
}

bool TagFilter::filterObject(
    const TagsStack& tags_stack_) const noexcept {
  return !pimpl->expression->matches(false, tags_stack_);
}

} /* -- namespace OTest2 */

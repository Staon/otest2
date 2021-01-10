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
#include <runnerfiltertags.h>

#include <assert.h>
#include <cctype>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include <tags.h>
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
    TagExpressionException&& exc_) noexcept :
  Exception(std::move(exc_)),
  message(std::move(exc_.message)) {

}

TagExpressionException::~TagExpressionException() = default;

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
     * @brief Return true if the name and tags match the expression
     */
    virtual bool matches(
        const std::string& name_,
        const Tags& tags_) const noexcept = 0;
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
        const std::string& name_,
        const Tags& tags_) const noexcept override;
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
        const std::string& name_,
        const Tags& tags_) const noexcept override;
};

class TagExpressionName : public TagExpression {
  private:
    std::string name;

  public:
    explicit TagExpressionName(
        const std::string& name_);
    virtual ~TagExpressionName() = default;

    /* -- avoid copying */
    TagExpressionName(
        const TagExpressionName&) = delete;
    TagExpressionName& operator = (
        const TagExpressionName&) = delete;

    virtual bool matches(
        const std::string& name_,
        const Tags& tags_) const noexcept override;
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
        const std::string& name_,
        const Tags& tags_) const noexcept override;
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
        const std::string& name_,
        const Tags& tags_) const noexcept override;
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
        const std::string& name_,
        const Tags& tags_) const noexcept override;
};

TagExpressionOr::TagExpressionOr(
    TagExpressionPtr left_,
    TagExpressionPtr right_) :
  left(left_),
  right(right_) {

}

bool TagExpressionOr::matches(
    const std::string& name_,
    const Tags& tags_) const noexcept {
  return left->matches(name_, tags_) || right->matches(name_, tags_);
}

TagExpressionAnd::TagExpressionAnd(
    TagExpressionPtr left_,
    TagExpressionPtr right_) :
  left(left_),
  right(right_) {

}

bool TagExpressionAnd::matches(
    const std::string& name_,
    const Tags& tags_) const noexcept {
  return left->matches(name_, tags_) && right->matches(name_, tags_);
}

TagExpressionName::TagExpressionName(
    const std::string& name_) :
  name(name_) {

}

bool TagExpressionName::matches(
    const std::string& name_,
    const Tags& tags_) const noexcept {
  return name == name_;
}

TagExpressionTag::TagExpressionTag(
    const std::string& tag_) :
  tag(tag_) {

}

bool TagExpressionTag::matches(
    const std::string& name_,
    const Tags& tags_) const noexcept {
  return tags_.findTag(tag);
}

bool TagExpressionEmpty::matches(
    const std::string& name_,
    const Tags& tags_) const noexcept {
  return tags_.isEmpty();
}

TagExpressionNot::TagExpressionNot(
    TagExpressionPtr operand_) :
  operand(operand_) {

}

bool TagExpressionNot::matches(
    const std::string& name_,
    const Tags& tags_) const noexcept {
  return !operand->matches(name_, tags_);
}

class TagGlobPart {
  public:
    TagGlobPart() = default;
    virtual ~TagGlobPart() = default;

    /* -- avoid copying */
    TagGlobPart(
        const TagGlobPart&) = delete;
    TagGlobPart& operator = (
        const TagGlobPart&) = delete;

    virtual void transition(
        std::set<int>& current_states_,
        std::set<int>& new_states_,
        int state_index_,
        const std::string& name_,
        const Tags& tags_) const = 0;
};

typedef std::shared_ptr<TagGlobPart> TagGlobPartPtr;

class TagGlobPartExpression : public TagGlobPart {
  public:
    enum Repeating {
      ONCE,
      ZERO_MORE,
      ONCE_MORE,
    };

  private:
    TagExpressionPtr expr;
    Repeating repeating;

  public:
    explicit TagGlobPartExpression(
        TagExpressionPtr expr_,
        Repeating repeating_);
    virtual ~TagGlobPartExpression() = default;

    /* -- avoid copying */
    TagGlobPartExpression(
        const TagGlobPartExpression&) = delete;
    TagGlobPartExpression& operator = (
        const TagGlobPartExpression&) = delete;

    virtual void transition(
        std::set<int>& current_states_,
        std::set<int>& new_states_,
        int state_index_,
        const std::string& name_,
        const Tags& tags_) const override;
};

class TagGlobPartStar : public TagGlobPart {
  public:
    TagGlobPartStar() = default;
    virtual ~TagGlobPartStar() = default;

    /* -- avoid copying */
    TagGlobPartStar(
        const TagGlobPartStar&) = delete;
    TagGlobPartStar& operator = (
        const TagGlobPartStar&) = delete;

    virtual void transition(
        std::set<int>& current_states_,
        std::set<int>& new_states_,
        int state_index_,
        const std::string& name_,
        const Tags& tags_) const override;
};

class TagGlobPartDouble : public TagGlobPart {
  public:
    TagGlobPartDouble() = default;
    virtual ~TagGlobPartDouble() = default;

    /* -- avoid copying */
    TagGlobPartDouble(
        const TagGlobPartDouble&) = delete;
    TagGlobPartDouble& operator = (
        const TagGlobPartDouble&) = delete;

    virtual void transition(
        std::set<int>& current_states_,
        std::set<int>& new_states_,
        int state_index_,
        const std::string& name_,
        const Tags& tags_) const override;
};

TagGlobPartExpression::TagGlobPartExpression(
    TagExpressionPtr expr_,
    Repeating repeating_) :
  expr(expr_),
  repeating(repeating_) {

}

void TagGlobPartExpression::transition(
    std::set<int>& current_states_,
    std::set<int>& new_states_,
    int state_index_,
    const std::string& name_,
    const Tags& tags_) const {
  if(expr->matches(name_, tags_)) {
    new_states_.insert(state_index_ + 1);
    if(repeating != ONCE)
      new_states_.insert(state_index_);
  }

  /* -- always suppose I'm repeated zero times */
  if(repeating == ZERO_MORE)
    current_states_.insert(state_index_ + 1);
}

void TagGlobPartStar::transition(
    std::set<int>& current_states_,
    std::set<int>& new_states_,
    int state_index_,
    const std::string& name_,
    const Tags& tags_) const {
  new_states_.insert(state_index_ + 1);
}

void TagGlobPartDouble::transition(
    std::set<int>& current_states_,
    std::set<int>& new_states_,
    int state_index_,
    const std::string& name_,
    const Tags& tags_) const {
  current_states_.insert(state_index_ + 1);
  new_states_.insert(state_index_);
  new_states_.insert(state_index_ + 1);
}

class TagGlob {
  private:
    std::vector<TagGlobPartPtr> globs;

  public:
    TagGlob() = default;
    virtual ~TagGlob() = default;

    /* -- avoid copying */
    TagGlob(
        const TagGlob&) = delete;
    TagGlob& operator = (
        const TagGlob&) = delete;

    /**
     * @brief Append part into the glob object
     */
    void appendPart(
        TagGlobPartPtr part_);

    /**
     * @brief Check whether a tag stack matches the glob
     *
     * @param tags_stack_ The tag stack
     * @return True if the stack matches
     */
    bool matches(
        const std::vector<TagsStack::TagRecord>& tags_stack_) const;
};

void TagGlob::appendPart(
    TagGlobPartPtr part_) {
  globs.push_back(part_);
}

bool TagGlob::matches(
    const std::vector<TagsStack::TagRecord>& tags_stack_) const {
  /* -- non-deterministic automaton */
  std::set<int> states_{0};
  for(const auto& tags_ : tags_stack_) {
    /* -- compute new generation of states */
    std::set<int> new_states_;
    while(!states_.empty()) {
      int state_index_(*states_.begin());
      states_.erase(states_.begin());
      if(state_index_ < globs.size())
        globs[state_index_]->transition(
            states_, new_states_, state_index_, tags_.name, tags_.tags);
    }

    /* -- switch new generation of states */
    states_.swap(new_states_);
    new_states_.clear();
  }

  /* -- if the final state has been reached */
  return states_.find(static_cast<int>(globs.size())) != states_.end();
}

typedef std::shared_ptr<TagGlob> TagGlobPtr;

enum class TokenType {
  NONE,
  NAME,  /**< an object name */
  TAG,   /**< a tag name */
  CLASS, /**< a tag class name */
  NOT,   /**< not operator */
  AND,   /**< and operator */
  OR,    /**< or operator */
  LEFT,  /**< left parenthesis */
  RIGHT, /**< right parenthesis */
  QUAD,  /**< quad-dot */
  STAR,  /**< one glob star */
  DOUBLE,/**< double star */
  LBRACK,/**< left bracket */
  RBRACK,/**< right bracket */
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
      NAME,
      TAG_BEGIN,
      TAG,
      CLASS,
      OR,
      AND,
      QUAD,
      STAR,
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
            state = State::NAME;
            value.push_back(c_);
          }
          else if(c_ == '#') {
            state = State::TAG;
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
          else if(c_ == ':') {
            state = State::QUAD;
          }
          else if(c_ == '*') {
            state = State::STAR;
          }
          else if(c_ == '[') {
            return {TokenType::LBRACK, "["};
          }
          else if(c_ == ']') {
            return {TokenType::RBRACK, "]"};
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

      case State::NAME:
        if(std::istream::traits_type::not_eof(c_)) {
          if(std::isspace(c_)) {
            state = State::START;
            return resetValue(TokenType::NAME);
          }
          else if(std::isalnum(c_) || c_ == '_') {
            value.push_back(c_);
          }
          else if(c_ == '#') {
            state = State::TAG_BEGIN;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == '<') {
            state = State::CLASS;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == '!') {
            prepared = {TokenType::NOT, "!"};
            state = State::START;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == '|') {
            state = State::OR;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == '&') {
            state = State::AND;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == '(') {
            prepared = {TokenType::LEFT, "("};
            state = State::START;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == ')') {
            prepared = {TokenType::RIGHT, ")"};
            state = State::START;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == ':') {
            state = State::QUAD;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == '*') {
            state = State::STAR;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == '[') {
            prepared = {TokenType::LBRACK, "["};
            state = State::START;
            return resetValue(TokenType::NAME);
          }
          else if(c_ == ']') {
            prepared = {TokenType::RBRACK, "]"};
            state = State::START;
            return resetValue(TokenType::NAME);
          }
          else {
            throw TagExpressionException(location, c_);
          }
          break;
        }
        else {
          /* -- end of string -> return the parsed tag */
          state = State::START;
          return resetValue(TokenType::NAME);
        }

      case State::TAG_BEGIN:
        if(std::istream::traits_type::not_eof(c_)) {
          if(std::isalpha(c_)) {
            state = State::TAG;
            value.push_back(c_);
            break;
          }
        }
        throw TagExpressionException(location, c_);

      case State::TAG:
        if(std::istream::traits_type::not_eof(c_)) {
          if(std::isspace(c_)) {
            state = State::START;
            return resetValue(TokenType::TAG);
          }
          else if(std::isalnum(c_) || c_ == '_' || c_ == '-' || c_ == '.') {
            value.push_back(c_);
          }
          else if(c_ == '#') {
            state = State::TAG_BEGIN;
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
          else if(c_ == ':') {
            state = State::QUAD;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == '*') {
            state = State::STAR;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == '[') {
            prepared = {TokenType::LBRACK, "["};
            state = State::START;
            return resetValue(TokenType::TAG);
          }
          else if(c_ == ']') {
            prepared = {TokenType::RBRACK, "]"};
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

      case State::QUAD:
        if(std::istream::traits_type::not_eof(c_)) {
          if(c_ == ':') {
            state = State::START;
            return {TokenType::QUAD, "::"};
          }
          else {
            throw TagExpressionException(location, c_);
          }
        }
        else {
          throw TagExpressionException("unexpected end of the expression");
        }

      case State::STAR:
        if(std::istream::traits_type::not_eof(c_)) {
          if(std::isspace(c_)) {
            /* -- ignore spaces */
            state = State::START;
            return {TokenType::STAR, "*"};
          }
          else if(std::isalpha(c_) || c_ == '_') {
            state = State::NAME;
            value.push_back(c_);
            return {TokenType::STAR, "*"};
          }
          else if(c_ == '#') {
            state = State::TAG_BEGIN;
            return {TokenType::STAR, "*"};
          }
          else if(c_ == '<') {
            state = State::CLASS;
            return {TokenType::STAR, "*"};
          }
          else if(c_ == '!') {
            prepared = {TokenType::NOT, "!"};
            state = State::START;
            return {TokenType::STAR, "*"};
          }
          else if(c_ == '|') {
            state = State::OR;
            return {TokenType::STAR, "*"};
          }
          else if(c_ == '&') {
            state = State::AND;
            return {TokenType::STAR, "*"};
          }
          else if(c_ == '(') {
            prepared = {TokenType::LEFT, "("};
            state = State::START;
            return {TokenType::STAR, "*"};
         }
          else if(c_ == ')') {
            prepared = {TokenType::RIGHT, ")"};
            state = State::START;
            return {TokenType::STAR, "*"};
          }
          else if(c_ == ':') {
            state = State::QUAD;
            return {TokenType::STAR, "*"};
          }
          else if(c_ == '*') {
            state = State::START;
            return {TokenType::DOUBLE, "**"};
          }
          else if(c_ == '[') {
            prepared = {TokenType::LBRACK, "["};
            state = State::START;
            return resetValue(TokenType::STAR);
          }
          else if(c_ == ']') {
            prepared = {TokenType::RBRACK, "]"};
            state = State::START;
            return resetValue(TokenType::STAR);
          }
          else {
            /* -- invalid character */
            throw TagExpressionException(location, c_);
          }
        }
        else {
          state = State::START;
          return {TokenType::STAR, "*"};
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

    TagGlobPtr glob();
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

    TagGlobPtr parse();
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

TagGlobPtr SyntaxParser::glob() {
  TagGlobPtr glob_(std::make_shared<TagGlob>());

  /* -- empty glob -> accept all */
  Token l_(lookAhead());
  if(l_.type == TokenType::END) {
    glob_->appendPart(std::make_shared<TagGlobPartDouble>());
    return glob_;
  }

  /* -- sequence of expressions, stars or double stars */
  while(true) {
    if(l_.type == TokenType::STAR) {
      glob_->appendPart(std::make_shared<TagGlobPartStar>());
      compare(TokenType::STAR);
    }
    else if(l_.type == TokenType::DOUBLE) {
      glob_->appendPart(std::make_shared<TagGlobPartDouble>());
      compare(TokenType::DOUBLE);
    }
    else if(l_.type == TokenType::LBRACK) {
      compare(TokenType::LBRACK);
      TagExpressionPtr expr_(expression());
      compare(TokenType::RBRACK);
      l_ = lookAhead();
      if(l_.type == TokenType::STAR) {
        compare(TokenType::STAR);
        glob_->appendPart(std::make_shared<TagGlobPartExpression>(
            expr_, TagGlobPartExpression::ZERO_MORE));
      }
      else {
        glob_->appendPart(std::make_shared<TagGlobPartExpression>(
            expr_, TagGlobPartExpression::ONCE_MORE));
      }
    }
    else {
      TagExpressionPtr expr_(expression());
      glob_->appendPart(std::make_shared<TagGlobPartExpression>(
          expr_, TagGlobPartExpression::ONCE));
    }

    /* -- end of the glob */
    l_ = lookAhead();
    if(l_.type == TokenType::END)
      break;

    /* -- move to next */
    compare(TokenType::QUAD);
    l_ = lookAhead();
  }

  return glob_;
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
  if(l_.type == TokenType::NAME) {
    compare(TokenType::NAME);
    return std::make_shared<TagExpressionName>(l_.value);
  }
  else if(l_.type == TokenType::TAG) {
    compare(TokenType::TAG);
    return std::make_shared<TagExpressionTag>(l_.value);
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

TagGlobPtr SyntaxParser::parse() {
  TagGlobPtr glob_(glob());
  compare(TokenType::END);
  return glob_;
}

} /* -- namespace */

struct RunnerFilterTags::Impl {
    TagGlobPtr glob;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        const std::string& tag_expression_);
    ~Impl();
};

RunnerFilterTags::Impl::Impl(
    const std::string& tag_expression_) {
  std::istringstream iss_(tag_expression_);
  Lexan lexan_(&iss_);
  SyntaxParser parser_(&lexan_);
  glob = parser_.parse();
}

RunnerFilterTags::Impl::~Impl() {

}

RunnerFilterTags::RunnerFilterTags() :
  pimpl(new Impl("")) {

}

RunnerFilterTags::RunnerFilterTags(
    const std::string& tag_expression_) :
  pimpl(new Impl(tag_expression_)) {

}

RunnerFilterTags::~RunnerFilterTags() {
  odelete(pimpl);
}

bool RunnerFilterTags::filterPath(
    const TagsStack& path_) const noexcept {
  std::vector<TagsStack::TagRecord> tags_;
  path_.fillTags(tags_);
  return !pimpl->glob->matches(tags_);
}

} /* -- namespace OTest2 */

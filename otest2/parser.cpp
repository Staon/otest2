#include "parser.h"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <iostream>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <sstream>
#include <string>

#include "error.h"
#include "excparser.h"
#include "generatorprinter.h"
#include "location.h"

namespace OTest2 {

const std::string SUITE_ANNOTATION("otest2::suite");
const std::string START_UP_MARK("OTest2::StartUpMark");
const std::string TEAR_DOWN_MARK("OTest2::TearDownMark");
const std::string CASE_MARK("OTest2::CaseMark");
const std::string STATE_MARK("OTest2::StateMark");
const std::string ANONYMOUS_STATE("AnonymousState");

Location createLocation(
    clang::SourceManager* srcmgr_,
    const clang::SourceLocation& loc_) {
  clang::PresumedLoc ploc_(srcmgr_->getPresumedLoc(loc_));
  return Location(ploc_.getLine(), ploc_.getColumn());
}

template<typename Node_>
clang::SourceRange getNodeRange(
    clang::SourceManager* srcmgr_,
    clang::LangOptions* langopts_,
    Node_* node_) {
  clang::SourceRange range_(node_->getSourceRange());

  /* -- adjust range at the end of last token */
  clang::SourceLocation true_end_(
      clang::Lexer::getLocForEndOfToken(
          range_.getEnd(),
          0,
          *srcmgr_,
          *langopts_));

  return clang::SourceRange(range_.getBegin(), true_end_);
}

struct ParserContext {
    /* -- error reporting */
    bool* failure;
    ParserException* exception;

    /* -- parser state */
    enum State {
      ROOT = 0,
      SUITE_VAR,
      SUITE_START_UP,
      SUITE_START_UP_END,
      SUITE_TEAR_DOWN,
      SUITE_TEAR_DOWN_END,
      CASE_BEGIN,
      CASE_VAR,
      CASE_START_UP,
      CASE_START_UP_END,
      CASE_TEAR_DOWN,
      CASE_TEAR_DOWN_END,
      CASE_NEXT_STATE,
      CASE_FINISHED,
      STATE_BEGIN,
    };
    State state;
    clang::ASTContext* comp_context;
    clang::SourceManager* srcmgr;
    clang::LangOptions* langopts;

    /* -- output generation */
    Generator* generator;
    Location last_location;

    explicit ParserContext(
        clang::ASTContext* comp_context_,
        clang::SourceManager* srcmgr_,
        clang::LangOptions* langopts_,
        Generator* generator_,
        bool* failure_,
        ParserException* exception_);

    template<typename Node_>
    void setError(
        const std::string& message_,
        Node_* node_);
    template<typename Node_>
    void copyInput(
        Node_* node_,
        bool end_ = false);
    template<typename Node_>
    void copyInputOfNode(
        Node_* node_);
    template<typename Node_>
    void moveToEnd(
        Node_* node_);
};


ParserContext::ParserContext(
    clang::ASTContext* comp_context_,
    clang::SourceManager* srcmgr_,
    clang::LangOptions* langopts_,
    Generator* generator_,
    bool* failure_,
    ParserException* exception_) :
  failure(failure_),
  exception(exception_),
  state(ROOT),
  comp_context(comp_context_),
  srcmgr(srcmgr_),
  langopts(langopts_),
  generator(generator_),
  last_location() {

}

template<typename Node_>
void ParserContext::setError(
    const std::string& message_,
    Node_* node_) {
  /* -- get error location */
  clang::SourceLocation beg_(node_->getLocStart());
  clang::SourceLocation end_(node_->getLocEnd());
  clang::PresumedLoc pbeg_(srcmgr->getPresumedLoc(beg_));
  clang::PresumedLoc pend_(srcmgr->getPresumedLoc(end_));

  /* -- fill the exception */
  *failure = true;
  exception->setException(
      message_,
      pbeg_.getFilename(),
      pbeg_.getLine(),
      pbeg_.getColumn(),
      pend_.getLine(),
      pend_.getColumn());
}

template<typename Node_>
void ParserContext::copyInput(
    Node_* node_,
    bool end_) {
  /* -- get node's location */
  clang::SourceRange range_(getNodeRange(srcmgr, langopts, node_));
  clang::SourceLocation loc_;
  if(!end_)
    loc_ = range_.getBegin();
  else
    loc_ = range_.getEnd();
  Location l2_(createLocation(srcmgr, loc_));

  /* -- get content of the input file */
  if(last_location < l2_) {
    generator->copySource(last_location, l2_);
  }
}

template<typename Node_>
void ParserContext::copyInputOfNode(
    Node_* node_) {
  clang::SourceRange range_(getNodeRange(srcmgr, langopts, node_));
  generator->copySource(
      createLocation(srcmgr, range_.getBegin()),
      createLocation(srcmgr, range_.getEnd()));
}

template<typename Node_>
void ParserContext::moveToEnd(
    Node_* node_) {
  clang::SourceRange range_(getNodeRange(srcmgr, langopts, node_));
  last_location = createLocation(srcmgr, range_.getEnd());
}

class SuiteVisitor : public clang::RecursiveASTVisitor<SuiteVisitor> {
  private:
    ParserContext* context;

    clang::VarDecl* getVarDecl(
        clang::Stmt* stmt_);
    clang::CompoundStmt* getCompound(
        clang::Stmt* stmt_);

    bool parseVariable(
        clang::VarDecl* vardecl_);
    bool parseCaseBody(
        clang::Stmt* stmt_);
    bool parseSuiteBody(
        clang::Stmt* stmt_);
    bool parseSuite(
        clang::FunctionDecl* fce_);

  public:
    explicit SuiteVisitor(
        ParserContext* context_);
    virtual ~SuiteVisitor();

    bool VisitFunctionDecl(
        clang::FunctionDecl* fce_);
};

SuiteVisitor::SuiteVisitor(
    ParserContext* context_) :
  context(context_) {

}

SuiteVisitor::~SuiteVisitor() {

}

clang::VarDecl* SuiteVisitor::getVarDecl(
    clang::Stmt* stmt_) {
  /* -- check the declaration statement */
  if(!clang::isa<clang::DeclStmt>(stmt_)) {
    context->setError("invalid declaration statement", stmt_);
    return nullptr;
  }

  /* -- there must be exactly one variable */
  clang::DeclStmt* declstmt_(clang::cast<clang::DeclStmt>(stmt_));
  if(declstmt_->decl_begin() == declstmt_->decl_end()
     || !clang::isa<clang::VarDecl>(*declstmt_->decl_begin())
     || (declstmt_->decl_begin() + 1) != declstmt_->decl_end()) {
    context->setError("invalid declaration statement", declstmt_);
    return nullptr;
  }
  return clang::cast<clang::VarDecl>(*declstmt_->decl_begin());
}

clang::CompoundStmt* SuiteVisitor::getCompound(
    clang::Stmt* stmt_) {
  if(!clang::isa<clang::CompoundStmt>(stmt_)) {
    context->setError("not compound statement", stmt_);
    return nullptr;
  }
  return clang::cast<clang::CompoundStmt>(stmt_);
}

bool SuiteVisitor::parseVariable(
    clang::VarDecl* vardecl_) {
  std::string varname_(vardecl_->getNameAsString());
  std::string type_(vardecl_->getType().getAsString());

  clang::Expr* init_(vardecl_->getInit());
  if(init_ == nullptr) {
    /* -- no initializer */
    context->generator->appendVariable(varname_, type_);
  }
  else {
    /* -- variable with initializer */
    if(vardecl_->getInitStyle() != clang::VarDecl::CallInit) {
      context->setError("only the callinit is supported", vardecl_);
      return false;
    }

    /* -- get range of the expression */
    clang::SourceRange range_(
        getNodeRange(context->srcmgr, context->langopts, init_));

    std::cout << init_->getStmtClassName() << std::endl;

    /* -- adjust the range for constructor call */
    if(clang::isa<clang::CXXConstructExpr>(init_)) {
      clang::CXXConstructExpr* ctrexpr_(
          clang::cast<clang::CXXConstructExpr>(init_));
      const int argnum_(ctrexpr_->getNumArgs());
      if(argnum_ <= 0) {
        range_ = clang::SourceRange(range_.getEnd(), range_.getEnd());
      }
      else {
        clang::SourceRange first_(
            getNodeRange(
                context->srcmgr,
                context->langopts,
                ctrexpr_->getArg(0)));
        clang::SourceRange last_(
            getNodeRange(
                context->srcmgr,
                context->langopts,
                ctrexpr_->getArg(argnum_ - 1)));
        range_ = clang::SourceRange(first_.getBegin(), last_.getEnd());
      }
    }

    /* -- notify the generator */
    context->generator->appendVariableInit(
        varname_,
        type_,
        createLocation(context->srcmgr, range_.getBegin()),
        createLocation(context->srcmgr, range_.getEnd()));
  }

  return true;
}

bool SuiteVisitor::parseCaseBody(
    clang::Stmt* stmt_) {
  for(
      auto iter_(stmt_->child_begin());
      iter_ != stmt_->child_end();
      ++iter_) {
    switch(context->state) {
      case ParserContext::CASE_VAR: {
        if(clang::isa<clang::CompoundStmt>(*iter_)) {
          /* -- anonymous state */
          context->state = ParserContext::CASE_FINISHED;
          context->generator->caseStartUp();
          context->generator->emptyBody();
          context->generator->caseTearDown();
          context->generator->emptyBody();
          context->generator->enterState(ANONYMOUS_STATE);
          context->copyInputOfNode(*iter_);
          context->generator->leaveState();
          break;
        }
        else {
          clang::VarDecl* vardecl_(getVarDecl(*iter_));
          if(vardecl_ == nullptr)
            return false;
          std::string type_(vardecl_->getType().getAsString());

          if(type_ == START_UP_MARK) {
            /* -- switch state and notify the generator */
            context->state = ParserContext::CASE_START_UP;
            context->generator->caseStartUp();
            break;
          }

          if(type_ == TEAR_DOWN_MARK) {
            /* -- switch state and notify the generator */
            context->state = ParserContext::CASE_TEAR_DOWN;
            context->generator->caseStartUp();
            context->generator->emptyBody();
            context->generator->caseTearDown();
            break;
          }

          if(type_ == STATE_MARK) {
            context->state = ParserContext::CASE_BEGIN;
            context->generator->caseStartUp();
            context->generator->emptyBody();
            context->generator->caseTearDown();
            context->generator->emptyBody();
            context->generator->enterState(vardecl_->getNameAsString());
            break;
          }

          /* -- case variable */
          if(!parseVariable(vardecl_))
            return false;

          break;
        }
      }

      case ParserContext::CASE_START_UP: {
        clang::CompoundStmt* block_(getCompound(*iter_));
        if(block_ == nullptr)
          return false;
        context->state = ParserContext::CASE_START_UP_END;
        context->copyInputOfNode(block_);
        context->generator->leaveState();
        break;
      }

      case ParserContext::CASE_START_UP_END: {
        if(clang::isa<clang::CompoundStmt>(*iter_)) {
          /* -- anonymous state */
          context->state = ParserContext::CASE_FINISHED;
          context->generator->caseTearDown();
          context->generator->emptyBody();
          context->generator->enterState(ANONYMOUS_STATE);
          context->copyInputOfNode(*iter_);
          break;
        }
        else {
          clang::VarDecl* vardecl_(getVarDecl(*iter_));
          if(vardecl_ == nullptr)
            return false;
          std::string type_(vardecl_->getType().getAsString());

          if(type_ == TEAR_DOWN_MARK) {
            /* -- switch state and notify the generator */
            context->state = ParserContext::CASE_TEAR_DOWN;
            context->generator->caseTearDown();

            continue;
          }

          if(type_ == STATE_MARK) {
            context->state = ParserContext::STATE_BEGIN;
            context->generator->caseTearDown();
            context->generator->emptyBody();
            context->generator->enterState(vardecl_->getNameAsString());

            continue;
          }

          context->setError("invalid suite item", *iter_);
          return false;
        }
      }

      case ParserContext::CASE_TEAR_DOWN: {
        clang::CompoundStmt* block_(getCompound(*iter_));
        if(block_ == nullptr)
          return false;
        context->state = ParserContext::CASE_TEAR_DOWN_END;
        context->copyInputOfNode(block_);
        break;
      }

      case ParserContext::CASE_TEAR_DOWN_END: {
        if(clang::isa<clang::CompoundStmt>(*iter_)) {
          /* -- anonymous state */
          context->state = ParserContext::CASE_FINISHED;
          context->generator->enterState(ANONYMOUS_STATE);
          context->copyInputOfNode(*iter_);
          context->generator->leaveState();
          break;
        }
        else {
          clang::VarDecl* vardecl_(getVarDecl(*iter_));
          if(vardecl_ == nullptr)
            return false;
          std::string type_(vardecl_->getType().getAsString());

          if(type_ == STATE_MARK) {
            context->state = ParserContext::STATE_BEGIN;
            context->generator->enterState(vardecl_->getNameAsString());

            continue;
          }

          context->setError("invalid suite item", *iter_);
          return false;
        }
      }

      case ParserContext::STATE_BEGIN: {
        clang::CompoundStmt* block_(getCompound(*iter_));
        if(block_ == nullptr)
          return false;
        context->copyInputOfNode(*iter_);
        context->generator->leaveState();
        context->state = ParserContext::CASE_TEAR_DOWN_END;
        break;
      }

      default:
        context->setError("invalid suite item", *iter_);
        return false;
    }
  }

  /* -- handle empty suite */
  switch(context->state) {
    case ParserContext::CASE_VAR:
      context->generator->caseStartUp();
      context->generator->emptyBody();
      /* -- missing break is expected */
    case ParserContext::CASE_START_UP_END:
      context->generator->caseTearDown();
      context->generator->emptyBody();
      break;
    case ParserContext::CASE_TEAR_DOWN_END:
    case ParserContext::CASE_FINISHED:
      break;
    default:
      context->setError("invalid format of the case", stmt_);
      return false;
  }

  return true;
}

bool SuiteVisitor::parseSuiteBody(
    clang::Stmt* stmt_) {
  for(
      auto iter_(stmt_->child_begin());
      iter_ != stmt_->child_end();
      ++iter_) {
    switch(context->state) {
      case ParserContext::SUITE_VAR: {
        clang::VarDecl* vardecl_(getVarDecl(*iter_));
        if(vardecl_ == nullptr)
          return false;
        std::string type_(vardecl_->getType().getAsString());

        if(type_ == START_UP_MARK) {
          /* -- switch state and notify the generator */
          context->state = ParserContext::SUITE_START_UP;
          context->generator->suiteStartUp();
          break;
        }

        if(type_ == TEAR_DOWN_MARK) {
          /* -- switch state and notify the generator */
          context->state = ParserContext::SUITE_TEAR_DOWN;
          context->generator->suiteStartUp();
          context->generator->emptyBody();
          context->generator->suiteTearDown();
          break;
        }

        if(type_ == CASE_MARK) {
          context->state = ParserContext::CASE_BEGIN;
          context->generator->suiteStartUp();
          context->generator->emptyBody();
          context->generator->suiteTearDown();
          context->generator->emptyBody();
          context->generator->enterCase(vardecl_->getNameAsString());
          break;
        }

        /* -- suite variable */
        if(!parseVariable(vardecl_))
          return false;

        break;
      }

      case ParserContext::SUITE_START_UP: {
        clang::CompoundStmt* block_(getCompound(*iter_));
        if(block_ == nullptr)
          return false;
        context->state = ParserContext::SUITE_START_UP_END;
        context->copyInputOfNode(block_);
        break;
      }

      case ParserContext::SUITE_START_UP_END: {
        clang::VarDecl* vardecl_(getVarDecl(*iter_));
        if(vardecl_ == nullptr)
          return false;
        std::string type_(vardecl_->getType().getAsString());

        if(type_ == TEAR_DOWN_MARK) {
          /* -- switch state and notify the generator */
          context->state = ParserContext::SUITE_TEAR_DOWN;
          context->generator->suiteTearDown();

          continue;
        }

        if(type_ == CASE_MARK) {
          context->state = ParserContext::CASE_BEGIN;
          context->generator->suiteTearDown();
          context->generator->emptyBody();
          context->generator->enterCase(vardecl_->getNameAsString());

          continue;
        }

        context->setError("invalid suite item", *iter_);
        return false;
      }

      case ParserContext::SUITE_TEAR_DOWN: {
        clang::CompoundStmt* block_(getCompound(*iter_));
        if(block_ == nullptr)
          return false;
        context->state = ParserContext::SUITE_TEAR_DOWN_END;
        context->copyInputOfNode(block_);
        break;
      }

      case ParserContext::SUITE_TEAR_DOWN_END: {
        clang::VarDecl* vardecl_(getVarDecl(*iter_));
        if(vardecl_ == nullptr)
          return false;
        std::string type_(vardecl_->getType().getAsString());

        if(type_ == CASE_MARK) {
          context->state = ParserContext::CASE_BEGIN;
          context->generator->enterCase(vardecl_->getNameAsString());

          continue;
        }

        context->setError("invalid suite item", *iter_);
        return false;
      }

      case ParserContext::CASE_BEGIN: {
        clang::CompoundStmt* block_(getCompound(*iter_));
        if(block_ == nullptr)
          return false;
        context->state = ParserContext::CASE_VAR;
        if(!parseCaseBody(*iter_))
          return false;
        context->generator->leaveCase();
        context->state = ParserContext::SUITE_TEAR_DOWN_END;
        break;
      }

      default:
        context->setError("invalid suite item", *iter_);
        return false;
    }
  }

  /* -- handle empty suite */
  switch(context->state) {
    case ParserContext::SUITE_VAR:
      context->generator->suiteStartUp();
      context->generator->emptyBody();
      /* -- missing break is expected */
    case ParserContext::SUITE_START_UP_END:
      context->generator->suiteTearDown();
      context->generator->emptyBody();
      break;
    case ParserContext::SUITE_TEAR_DOWN_END:
      break;
    default:
      context->setError("invalid format of the suite", stmt_);
      return false;
  }

  return true;
}

bool SuiteVisitor::parseSuite(
    clang::FunctionDecl* fce_) {
  if(!fce_->hasBody()) {
    context->setError("the suite must have a body", fce_);
    return false;
  }

  /* -- copy the input file */
  context->copyInput(fce_);

  /* -- enter the suite */
  std::string suitename_(fce_->getNameAsString());
  context->generator->enterSuite(suitename_);

  /* -- parse body of the suite */
  context->state = ParserContext::SUITE_VAR;
  clang::Stmt* body_(fce_->getBody());
  if(!parseSuiteBody(body_))
    return false;
  context->state = ParserContext::ROOT;

  /* -- leave the suite */
  context->generator->leaveSuite();

  /* -- skip the body of the function */
  context->moveToEnd(fce_);

  return true;
}

bool SuiteVisitor::VisitFunctionDecl(
    clang::FunctionDecl* fce_) {
  if(fce_->hasAttrs()) {
    auto iter_(fce_->attr_begin());
    for(
        auto iter_(fce_->attr_begin());
        iter_ != fce_->attr_end();
        ++iter_) {
      if((*iter_)->getKind() == clang::attr::Annotate) {
        auto sa_(clang::cast<clang::AnnotateAttr>(*iter_));
        auto str_(sa_->getAnnotation());
        if(str_.data() == SUITE_ANNOTATION) {
          if(!parseSuite(fce_))
            return false;
        }
      }
    }
  }

  return true;
}

class ParserConsumer : public clang::ASTConsumer {
  private:
    ParserContext context;
    SuiteVisitor visitor;

  public:
    explicit ParserConsumer(
        Generator* generator_,
        bool* failure_,
        ParserException* exception_,
        clang::ASTContext* comp_context_,
        clang::SourceManager* srcmgr_,
        clang::LangOptions* langopts_);
    virtual ~ParserConsumer();

    void HandleTranslationUnit(
        clang::ASTContext& context_) final;
};

ParserConsumer::ParserConsumer(
    Generator* generator_,
    bool* failure_,
    ParserException* exception_,
    clang::ASTContext* comp_context_,
    clang::SourceManager* srcmgr_,
    clang::LangOptions* langopts_) :
  context(comp_context_, srcmgr_, langopts_, generator_, failure_, exception_),
  visitor(&context) {

}


ParserConsumer::~ParserConsumer() {

}

void ParserConsumer::HandleTranslationUnit(
    clang::ASTContext& context_) {
  /* -- opening braces of the file */
  context.generator->beginFile();

  /* -- traverse the tree */
  visitor.TraverseDecl(context_.getTranslationUnitDecl());

  /* -- close the file */
  context.generator->endFile(context.last_location);
}

class ParserAction : public clang::ASTFrontendAction {
  private:
    Generator* generator;
    bool* failure;
    ParserException* exception;

  public:
    explicit ParserAction(
        Generator* generator_,
        bool* failure_,
        ParserException* exception_);
    virtual ~ParserAction();

    /* -- avoid copying */
    ParserAction(
        const ParserAction&) = delete;
    ParserAction& operator =(
        const ParserAction&) = delete;

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& ci_,
        clang::StringRef) final;
};

ParserAction::ParserAction(
    Generator* generator_,
    bool* failure_,
    ParserException* exception_) :
  generator(generator_),
  failure(failure_),
  exception(exception_) {

}

ParserAction::~ParserAction() {

}

std::unique_ptr<clang::ASTConsumer> ParserAction::CreateASTConsumer(
    clang::CompilerInstance& ci_,
    clang::StringRef strref_) {
  return std::make_unique<ParserConsumer>(
      generator,
      failure,
      exception,
      &ci_.getASTContext(),
      &ci_.getSourceManager(),
      &ci_.getLangOpts());
}

class FrontendFactory : public clang::tooling::FrontendActionFactory {
  private:
    Generator* generator;
    bool* failure;
    ParserException* exception;

  public:
    explicit FrontendFactory(
        Generator* generator_,
        bool* failure_,
        ParserException* exception_);
    virtual ~FrontendFactory();

    clang::FrontendAction* create();
};

FrontendFactory::FrontendFactory(
    Generator* generator_,
    bool* failure_,
    ParserException* exception_) :
  generator(generator_),
  failure(failure_),
  exception(exception_) {

}

FrontendFactory::~FrontendFactory() {

}

clang::FrontendAction* FrontendFactory::create() {
  return new ParserAction(generator, failure, exception);
}

/* -- command line options */
llvm::cl::OptionCategory ParserOptCategory("otest2 options");
llvm::cl::extrahelp CommonHelp(
    clang::tooling::CommonOptionsParser::HelpMessage);
llvm::cl::extrahelp MoreHelp("\nOTest2 preprocessor");

void parse(
    const std::string& filename_) {
  /* -- parse the file */
  const char* argv_[] = {
      "otest2",
      "--extra-arg=--std=c++11",
      "--extra-arg=-DOTEST2_PARSER_ACTIVE",
      filename_.c_str()
  };
  int argc_(sizeof(argv_) / sizeof(const char*));
  clang::tooling::CommonOptionsParser options_(argc_, argv_, ParserOptCategory);
  clang::tooling::ClangTool tool_(
      options_.getCompilations(), options_.getSourcePathList());
  GeneratorPrinter generator_;
  bool failure_(false);
  ParserException exception_;
  tool_.run(new FrontendFactory(&generator_, &failure_, &exception_));

  /* -- report an error */
  if(failure_)
    throw ParserException(std::move(exception_));
}

} /* -- namespace OTest2 */

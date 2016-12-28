%expect 4

%code top {
/* -- turn on parser's debugging */
#define YYDEBUG 1

#include <ondrart/bisonpragmas.h>

}

/* -- generated parser */
%skeleton "lalr1.cc"  /* -- C++ class */
%define filename_type "dstring" /* -- don't use std::string */
%parse-param { ::OTest2::ParserContext* parser_context }
%lex-param { ::OTest2::ParserContext* parser_context }

%code requires {

#include <cstdint> 
#include <datstr/dstring.h>
#include <iostream>

#include "declaration.h"
#include "declargs.h"
#include "declarray.h"
#include "declcore.h"
#include "declfunction.h"
#include "declleft.h"
#include "declmemptr.h"
#include "declpointer.h"
#include "declreference.h"
#include "decltype.h"

namespace OTest2 {

class ParserContext;

} /* -- namespace OTest2 */

}

/* -- semantic value */
%union {
  dstring* string;
  ::OTest2::DeclLeft* declleft;
  ::OTest2::DeclCore* declcore;
  ::OTest2::DeclArgs* declargs;
  ::OTest2::Declaration* declaration;
  std::uint8_t modifier;
}

/* -- lexical elements */
%token C_SUITE
%token C_CASE
%token C_STATE
%token C_CTOR
%token C_DTOR
%token C_NAMESPACE
%token C_IDENTIFIER
%token C_LPARENT
%token C_RPARENT
%token C_LBRACKET
%token C_RBRACKET
%token C_LCURLY
%token C_RCURLY
%token C_QUADDOT
%token C_MEMPTR
%token C_STRING
%token C_SPACE
%token C_ANY
%token C_CONST
%token C_ASTERISK
%token C_REFERENCE
%token C_SEMICOLON
%token C_COMMA
%token C_LT
%token C_GT

%type<declcore> Declaration
%type<declleft> LeftDecl
%type<declcore> DeclarationInner
%type<declleft> LeftDeclInner
%type<declleft> LeftTypeItem
%type<modifier> DeclModifier
%type<declcore> CoreDecl
%type<declaration> RightDecl
%type<string> ArraySpec
%type<declargs> FceArgs
%type<declargs> FceArgsList
%type<string> QualifiedName
%type<string> QualifiedNameRaw
%type<string> Identifier
%type<string> IdentifierOrTemplate
%type<string> C_IDENTIFIER
%type<string> C_STRING
%type<string> C_SPACE
%type<string> C_ANY

%destructor { FREESTRING($$); } <string>
%destructor { delete $$; } <declaration>
%destructor { delete $$; } <declcore>
%destructor { delete $$; } <declargs>
%destructor { delete $$; } <declleft>

%code {

#include <iostream>
#include <otest/case.h>
#include "parsercontext.h"

/* -- macros used in the semantic actions */
#define CONTEXT parser_context
#define FREESTRING(string) (CONTEXT -> freeString(string))
#define PRINT(string_) (CONTEXT -> dumpString(string_))

/* -- forward declaration of the lexer */
namespace otest2 {
  int otest2lex(otest2::parser::semantic_type*, ::OTest2::ParserContext*);
}

}

%%

TestSource: /* -- epsilon */ { CONTEXT -> beginFile(); }
  | TestSource FreeBlock
  | TestSource NamespaceDef
  | TestSource SuiteDef
  ;

SuiteDef: Suite LParent Identifier RParent LCurly { CONTEXT -> enterSuite(*$3); } ObjectHeader Cases C_RCURLY { CONTEXT -> leaveSuite(); }
  ;

Cases: /* -- epsilon */
  | Cases CaseDef
  ;

CaseDef: Case LParent Identifier RParent LCurly { CONTEXT -> enterCase(*$3); } ObjectHeader States RCurly { CONTEXT -> leaveCase(); }
  ;

States: StateList
  | NoStates
  ;
  
StateList: /* -- epsilon */
  | States StateDef
  ;

StateDef: State LParent Identifier RParent C_LCURLY { CONTEXT -> enterState(*$3); } FreeBody RCurly { CONTEXT -> leaveState(); }
  ;

NoStates: C_LCURLY { CONTEXT -> enterState("Init"); } FreeBody RCurly { CONTEXT -> leaveState(); }
  ;

ObjectHeader: Declarations { CONTEXT -> finishDeclarations(); } CtorOpt DtorOpt
  ;

CtorOpt: /* -- epsilon */
  | C_CTOR LParent LParent InitializersOpt RParent RParent C_LCURLY { CONTEXT -> startCatching(); } FreeBody RCurly {
      dstring* body_(CONTEXT -> stopCatching());
      CONTEXT -> setCtorBody(*body_);
      FREESTRING(body_);
    }
  ;

InitializersOpt: /* -- epsilon */
  | Initializers
  ;

Initializers: Initializer
  | Initializers Comma Initializer
  ;

Initializer: Identifier C_LPARENT { CONTEXT -> startCatching(); } FreeBody RParent { 
      dstring* body_(CONTEXT -> stopCatching());
      bool retval_(CONTEXT -> setInitializer(*$1, *body_));
      FREESTRING($1);
      FREESTRING(body_);
      if(!retval_) {
        YYABORT;
      }
    }
  ;

DtorOpt: /* -- epsilon */
  | C_DTOR LParent RParent C_LCURLY { CONTEXT -> startCatching(); } FreeBody RCurly {
      dstring* body_(CONTEXT -> stopCatching());
      CONTEXT -> setDtorBody(*body_);
      FREESTRING(body_);
    }
  ;

Declarations: /* -- epsilon */
  | Declarations Declaration Semicolon { 
      if($2 -> isAnonymous()) {
        delete $2;
        YYABORT;
      }
      else {
        CONTEXT -> appendVariable($2 -> getName(), $2 -> getDeclaration());
        delete $2;
      }
    }
  ;

Declaration: DeclModifier LeftDecl CoreDecl { $2 -> applyModifiersDeep($1); $<declleft>$ = $2; } RightDecl { $3 -> applyRightDecl($5); $$ = $3; }
  ;
  
LeftDecl: QualifiedName DeclModifier { $$ = new ::OTest2::DeclType(*$1); $$ -> applyModifiers($2); FREESTRING($1); }
  | LeftDecl LeftTypeItem DeclModifier { $2 -> applyLeftDecl($1); $2 -> applyModifiers($3); $$ = $2; }
  ;

DeclarationInner: LeftDeclInner CoreDecl { $<declleft>$ = $1; } RightDecl { $2 -> applyRightDecl($4); $$ = $2; }
  ;

LeftDeclInner: LeftTypeItem DeclModifier { $1 -> applyModifiers($2); $$ = $1; }
  | LeftDeclInner LeftTypeItem DeclModifier { $2 -> applyLeftDecl($1); $2 -> applyModifiers($3); $$ = $2; }
  ;

LeftTypeItem: Asterisk { $$ = new ::OTest2::DeclPointer; }
  | Reference { $$ = new ::OTest2::DeclReference; }
  | QualifiedName MemPtr { $$ = new ::OTest2::DeclMemPtr(*$1); FREESTRING($1); }
  ;

CoreDecl: /* -- epsilon */ { $$ = new ::OTest2::DeclCore; }
  | Identifier { $$ = new ::OTest2::DeclCore(*$1); FREESTRING($1); }
  | LParent RParent { $$ = new ::OTest2::DeclCore; }
  | LParent DeclarationInner RParent { $$ = $2; }
  ;

RightDecl: /* -- epsilon */ { $$ = $<declleft>0; }
  | ArraySpec { $<declleft>$ = $<declleft>0; } RightDecl { $$ = new ::OTest2::DeclArray(*$1, $3); FREESTRING($1); }
  | LParent FceArgs RParent { $<declleft>$ = $<declleft>0; } RightDecl { $$ = new ::OTest2::DeclFunction($5, $2); }
  ;

ArraySpec: C_LBRACKET { CONTEXT -> startCatching(); } FreeBody RBracket { $$ = CONTEXT -> stopCatching(); }
  ;

FceArgs: /* -- epsilon */ { $$ = new ::OTest2::DeclArgs; }
  | FceArgsList { $$ = $1; }
  ;

FceArgsList: Declaration { $$ = new ::OTest2::DeclArgs; $$ -> appendArgument($1); }
  | FceArgsList Comma Declaration { $1 -> appendArgument($3); $$ = $1; }
  ;

DeclModifier: /* -- epsilon */ { $$ = 0; }
  | DeclModifier Const { $$ = $1 | ::OTest2::Declaration::CONST; }
  ;

QualifiedName: QualifiedNameRaw { $$ = $1; }
  | QuadDot QualifiedNameRaw { $$ = CONTEXT -> allocateString(2, "::"); $$ -> Append(*$2); FREESTRING($2); } 
  ;

QualifiedNameRaw: IdentifierOrTemplate { $$ = $1; }
  | QualifiedNameRaw QuadDot IdentifierOrTemplate { $1 -> Append("::"); $1 -> Append(*$3); $$ = $1; FREESTRING($3); }
  ;

IdentifierOrTemplate: Identifier { $$ = $1; }
  | Identifier C_LT { CONTEXT -> startCatching(); } FreeBodyTempl GT {
      dstring* templ_(CONTEXT -> stopCatching());
      $1 -> Append("< ");
      $1 -> Append(*templ_);
      $1 -> Append(" >");
      FREESTRING(templ_); 
      $$ = $1; 
    }
  ;


NamespaceDef: Namespace C_LCURLY TestSource C_RCURLY
  ; 

FreeBody: /* -- epsilon */
  | FreeBody FreeBlock
  ;

FreeBlock: SpacePrint 
  | AnyPrint
  | QuadDotPrint
  | MemPtrPrint
  | IdentifierPrint
  | StringPrint
  | PairedCurlyPrint
  | PairedParentPrint
  | PairedBracketPrint
  | ConstPrint
  | AsteriskPrint
  | ReferencePrint
  | SemicolonPrint
  | CommaPrint
  | LTPrint
  | GTPrint
  ;

FreeBodyTempl: /* -- epsilon */
  | FreeBodyTempl FreeBlockTempl
  ;

FreeBlockTempl: SpacePrint 
  | AnyPrint
  | QuadDotPrint
  | MemPtrPrint
  | IdentifierPrint
  | StringPrint
  | PairedCurlyPrint
  | PairedParentPrint
  | PairedBracketPrint
  | PairedTemplPrint
  | ConstPrint
  | AsteriskPrint
  | ReferencePrint
  | SemicolonPrint
  | CommaPrint
  ;

PairedCurlyPrint: LCurlyPrint FreeBody RCurlyPrint
  ;

PairedParentPrint: LParentPrint FreeBody RParentPrint
  ;

PairedBracketPrint: LBracketPrint FreeBody RBracketPrint
  ;

PairedTemplPrint: LTPrint FreeBodyTempl GTPrint
  ;

Suite: C_SUITE IgnoredSpace
  ;

Case: C_CASE IgnoredSpace
  ;

State: C_STATE IgnoredSpace
  ;

Namespace: C_NAMESPACE IgnoredSpace
  ;
    
AnyPrint: C_ANY { PRINT(*$1); }
  ;

QuadDot: C_QUADDOT IgnoredSpace
  ;

QuadDotPrint: C_QUADDOT { PRINT("::"); }
  ;

MemPtr: C_MEMPTR IgnoredSpace
  ;

MemPtrPrint: C_MEMPTR { PRINT("::*"); }
  ;

Identifier: C_IDENTIFIER IgnoredSpace
  ;
  
IdentifierPrint: C_IDENTIFIER { PRINT(*$1); }
  ;

StringPrint: C_STRING { PRINT("\""); PRINT(*$1); PRINT("\""); }
  ;

LCurly: C_LCURLY IgnoredSpace
  ;
  
LCurlyPrint: C_LCURLY { PRINT("{"); }
  ;

RCurly: C_RCURLY IgnoredSpace
  ;

RCurlyPrint: C_RCURLY { PRINT("}"); }
  ;

LParent: C_LPARENT IgnoredSpace
  ;
  
LParentPrint: C_LPARENT { PRINT("("); }
  ;

RParent: C_RPARENT IgnoredSpace
  ;
  
RParentPrint: C_RPARENT { PRINT(")"); }
  ;

LBracketPrint: C_LBRACKET { PRINT("["); }
  ;

RBracket: C_RBRACKET IgnoredSpace
  ;

RBracketPrint: C_RBRACKET { PRINT("]"); }
  ;

Const: C_CONST IgnoredSpace
  ;
  
ConstPrint: C_CONST { PRINT("const"); }
  ;

Asterisk: C_ASTERISK IgnoredSpace
  ;
  
AsteriskPrint: C_ASTERISK { PRINT("*"); }
  ;

Reference: C_REFERENCE IgnoredSpace
  ;
  
ReferencePrint: C_REFERENCE { PRINT("&"); }
  ;

Semicolon: C_SEMICOLON IgnoredSpace
  ;
  
SemicolonPrint: C_SEMICOLON { PRINT(";"); }
  ;

Comma: C_COMMA IgnoredSpace
  ;
  
CommaPrint: C_COMMA { PRINT(","); }
  ;

LTPrint: C_LT { PRINT("<"); }
  ;

GT: C_GT IgnoredSpace
  ;

GTPrint: C_GT { PRINT(">"); }
  ;
  
IgnoredSpace: /* -- epsilon */
  | IgnoredSpace Space
  ;

Space: C_SPACE
  ;

SpacePrint: C_SPACE { PRINT(*$1); }
  ;

%%

#include "FlexLexer.h"

namespace otest2 {

void parser::error(
    const parser::location_type& loc_,
    const std::string& msg_) {
//  if(msg_ == "parse error")
//    parser_context -> setError(::OTest::Error::OTEST_SYNTAX);
//  else
//    parser_context -> setError(::OTest::Error::OTEST_CRITICAL, msg_.c_str());
}

int otest2lex(
    parser::semantic_type* lvalp_,
    ::OTest2::ParserContext* parser_context_) {
  return parser_context_ -> lexan -> yylex(lvalp_, parser_context_);
}

} /* -- namespace otest */

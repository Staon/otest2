%expect 1

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
#include <datstr/dstring.h>
#include <iostream>

namespace OTest2 {

class ParserContext;

} /* -- namespace OTest2 */

}

/* -- semantic value */
%union {
  dstring* string;
}

/* -- lexical elements */
%token C_SUITE
%token C_CASE
%token C_STATE
%token C_NAMESPACE
%token C_IDENTIFIER
%token C_LPARENT
%token C_RPARENT
%token C_LBRACKET
%token C_RBRACKET
%token C_LCURLY
%token C_RCURLY
%token C_QUADDOT
%token C_STRING
%token C_SPACE
%token C_ANY
%token C_CONST
%token C_ASTERISK
%token C_REFERENCE
%token C_SEMICOLON
%token C_COMMA

%type<string> Identifier
%type<string> C_IDENTIFIER
%type<string> C_STRING
%type<string> C_SPACE
%type<string> C_ANY

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

TestSource: /* -- epsilon */
  | TestSource FreeBlock
  | TestSource SpacePrint
  | TestSource NamespaceDef
  | TestSource SuiteDef
  ;

SuiteDef: Suite LParent Identifier RParent LCurly { CONTEXT -> enterSuite(*$3); } Declarations Cases C_RCURLY { CONTEXT -> leaveSuite(); }
  ;

Cases: /* -- epsilon */
  | Cases CaseDef
  ;

CaseDef: Case LParent Identifier RParent LCurly { CONTEXT -> enterCase(*$3); } Declarations States RCurly { CONTEXT -> leaveCase(); }
  ;

States: /* -- epsilon */
  | States StateDef
  ;

StateDef: State LParent Identifier RParent C_LCURLY { CONTEXT -> enterState(*$3); } FreeBody RCurly { CONTEXT -> leaveState(); }
  ;

Declarations: /* -- epsilon */
  | Declarations Declaration Semicolon;
  ;

Declaration: DeclModifier LeftDecl CoreDecl RightDecl
  ;
  
LeftDecl: QualifiedName DeclModifier
  | LeftTypeItem DeclModifier
  | LeftDecl LeftTypeItem DeclModifier
  ;

LeftTypeItem: Asterisk
  | Reference
  ;

CoreDecl: /* -- epsilon */
  | Identifier
  | LParent RParent
  | LParent Declaration RParent
  ;

RightDecl: /* -- epsilon */
  | RightDecl ArraySpec
  | RightDecl LParent FceArgs RParent
  ;

ArraySpec: C_LBRACKET FreeBody RBracket
  ;

FceArgs: /* -- epsilon */
  | FceArgsList
  ;

FceArgsList: Declaration
  | FceArgsList Comma Declaration
  ;

DeclModifier: /* -- epsilon */
  | Const
  ;

QualifiedName: QualifiedNameRaw
  | QuadDot QualifiedNameRaw
  ;

QualifiedNameRaw: Identifier
  | QualifiedNameRaw QuadDot Identifier
  ;

NamespaceDef: Namespace C_LCURLY TestSource C_RCURLY
  ; 

FreeBody: OptSpace
  | FreeBody FreeBlock OptSpace
  ;

FreeBlock: AnyPrint
  | QuadDotPrint
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
  ;

PairedCurlyPrint: LCurlyPrint FreeBody RCurlyPrint
  ;

PairedParentPrint: LParentPrint FreeBody RParentPrint
  ;

PairedBracketPrint: LBracketPrint FreeBody RBracketPrint
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
  
IgnoredSpace: /* -- epsilon */
  | IgnoredSpace Space
  ;

Space: C_SPACE
  ;

OptSpace: /* -- epsilon */
  | OptSpace SpacePrint
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

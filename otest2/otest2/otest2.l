
%{

#include "parsercontext.h"

#include <ondrart/flexpragmas.h>
#include <stdlib.h>
#include <string.h>


#define CONTEXT ((::OTest2::ParserContext*) parser_context)
#define NEWSTRING(len, text) (CONTEXT -> allocateString(len, text))

typedef otest2::parser::token token;

%}

/* -- don't use yywrap function - we use only one input buffer */
%option noyywrap
/* -- generate C++ generator object */
%option c++

%x SUITE
%x CASE
%x STATE
%x COMMENT
%x STRING
%x OBJECT
%x USING
%x ENVIRONMENT

%%

\/\*                            CONTEXT -> incCol(yyleng); yylval -> string = CONTEXT -> allocateString(yyleng, yytext); BEGIN(COMMENT);
<COMMENT>\*\/                   CONTEXT -> incCol(yyleng); BEGIN(0); yylval -> string -> Append(yytext); return token::C_SPACE;
<COMMENT>\n                     CONTEXT -> incLine(); yylval -> string -> Append(yytext);
<COMMENT>.                      CONTEXT -> incCol(yyleng); yylval -> string -> Append(yytext);

\/\/[^\n]*\n                    CONTEXT -> incLine(); yylval -> string = CONTEXT -> allocateString(yyleng, yytext); return token::C_SPACE;

\"                              CONTEXT -> incCol(yyleng); yylval -> string = CONTEXT -> allocateString(0, ""); BEGIN(STRING);
<STRING>\\\\                    CONTEXT -> incCol(yyleng); yylval -> string -> Append("\\\\");
<STRING>\\\"                    CONTEXT -> incCol(yyleng); yylval -> string -> Append("\\\"");
<STRING>\n                      CONTEXT -> incLine(); yylval -> string -> Append("\n");
<STRING>\"                      CONTEXT -> incCol(yyleng); BEGIN(0); return token::C_STRING;
<STRING>.                       CONTEXT -> incLine(); yylval -> string -> Append(yytext);

TEST_SUITE                      CONTEXT -> incCol(yyleng); return token::C_SUITE;
TEST_CASE                       CONTEXT -> incCol(yyleng); return token::C_CASE;
TEST_STATE                      CONTEXT -> incCol(yyleng); return token::C_STATE;
namespace                       CONTEXT -> incCol(yyleng); return token::C_NAMESPACE;
const                           CONTEXT -> incCol(yyleng); return token::C_CONST;
\*                              CONTEXT -> incCol(yyleng); return token::C_ASTERISK;
\&                              CONTEXT -> incCol(yyleng); return token::C_REFERENCE;
[_a-zA-Z][_a-zA-Z0-9]*          CONTEXT -> incCol(yyleng); yylval -> string = CONTEXT -> allocateString(yyleng, yytext); return token::C_IDENTIFIER;
\{                              CONTEXT -> incCol(yyleng); return token::C_LCURLY;
\}                              CONTEXT -> incCol(yyleng); return token::C_RCURLY;
\(                              CONTEXT -> incCol(yyleng); return token::C_LPARENT;
\)                              CONTEXT -> incCol(yyleng); return token::C_RPARENT;
\[                              CONTEXT -> incCol(yyleng); return token::C_LBRACKET;
\]                              CONTEXT -> incCol(yyleng); return token::C_RBRACKET;
::                              CONTEXT -> incCol(yyleng); return token::C_QUADDOT;
;                               CONTEXT -> incCol(yyleng); return token::C_SEMICOLON;
,                               CONTEXT -> incCol(yyleng); return token::C_COMMA;
[ \t\n]+                        CONTEXT -> incCol(yyleng); yylval -> string = CONTEXT -> allocateString(yyleng, yytext); return token::C_SPACE;
.                               CONTEXT -> incCol(yyleng); yylval -> string = CONTEXT -> allocateString(yyleng, yytext); return token::C_ANY;

<<EOF>>                    return 0;

%%
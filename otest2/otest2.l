
%{

#include "parsercontext.h"

#include <ondrart/flexpragmas.h>
#include <stdlib.h>
#include <string.h>


#define CONTEXT ((::OTest2::ParserContext*) parser_context)
#define NEWSTRING(len, text) (CONTEXT -> allocateString(len, text))
#define RETURN(token_) return CONTEXT -> returnToken(token::token_, yyleng, yyloc)

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

\/\*                            yyloc -> columns(yyleng); yylval -> string = CONTEXT -> allocateString(yyleng, yytext); BEGIN(COMMENT);
<COMMENT>\*\/                   BEGIN(0); yylval -> string -> Append(yytext); RETURN(C_SPACE);
<COMMENT>\n                     yyloc -> lines(); yylval -> string -> Append(yytext);
<COMMENT>.                      yyloc -> columns(yyleng); yylval -> string -> Append(yytext);

\/\/[^\n]*\n                    yyloc -> lines(); yylval -> string = CONTEXT -> allocateString(yyleng, yytext); return CONTEXT -> returnToken(token::C_SPACE, 0, yyloc);

\"                              yyloc -> columns(yyleng); yylval -> string = CONTEXT -> allocateString(0, ""); BEGIN(STRING);
<STRING>\\\\                    yyloc -> columns(yyleng); yylval -> string -> Append("\\\\");
<STRING>\\\"                    yyloc -> columns(yyleng); yylval -> string -> Append("\\\"");
<STRING>\n                      yyloc -> lines(); yylval -> string -> Append("\n");
<STRING>\"                      BEGIN(0); RETURN(C_STRING);
<STRING>.                       yyloc -> columns(yyleng); yylval -> string -> Append(yytext);

TEST_SUITE                      RETURN(C_SUITE);
TEST_CASE                       RETURN(C_CASE);
TEST_STATE                      RETURN(C_STATE);
TEST_CTOR                       RETURN(C_CTOR);
TEST_DTOR                       RETURN(C_DTOR);
namespace                       RETURN(C_NAMESPACE);
const                           RETURN(C_CONST);
\*                              RETURN(C_ASTERISK);
\&                              RETURN(C_REFERENCE);
[_a-zA-Z][_a-zA-Z0-9]*          yylval -> string = CONTEXT -> allocateString(yyleng, yytext); RETURN(C_IDENTIFIER);
\{                              RETURN(C_LCURLY);
\}                              RETURN(C_RCURLY);
\(                              RETURN(C_LPARENT);
\)                              RETURN(C_RPARENT);
\[                              RETURN(C_LBRACKET);
\]                              RETURN(C_RBRACKET);
::                              RETURN(C_QUADDOT);
::\*                            RETURN(C_MEMPTR);
;                               RETURN(C_SEMICOLON);
,                               RETURN(C_COMMA);
\<                              RETURN(C_LT);
\>                              RETURN(C_GT);
[ \t]+                          yylval -> string = CONTEXT -> allocateString(yyleng, yytext); RETURN(C_SPACE);
\n                              yyloc -> lines(); yylval -> string = CONTEXT -> allocateString(yyleng, yytext); return CONTEXT -> returnToken(token::C_SPACE, 0, yyloc);
.                               yylval -> string = CONTEXT -> allocateString(yyleng, yytext); RETURN(C_ANY);

<<EOF>>                    return 0;

%%
%{
#include <stdio.h>
#include <stdlib.h>
#include "t.tab.h"
%}

digit [0-9]
letter [A-Za-z]
id {letter}({letter}|{digit})*
number {digit}+

%%

">=" return GE;
"<=" return LE;
"true" return TRUE;
"false" return FALSE;
">" return GT;
"<" return LT;
"==" return EQ;
"!=" return NE;
"&&" return AND;
"||" return OR;
{id} {yylval.string = strdup(yytext); return ID;}
{number} {yylval.string = strdup(yytext);return NUM;}
";" return SEMI;
"(" return LB;
")" return RB;
" "

%%
%{
#include "y.tab.h"
#include "cons.h"
#include <stdlib.h>
%}

DIGIT [0-9]
SYMBOL [\!\=\-+*/a-zA-Z][a-zA-Z0-9]*

%%
[ \t\n]*             {}
"("                  { return O_PARENTH; }
")"                  { return C_PARENTH; }
"."                  { return DOT;       }
","                  { return COMMA;     }
"'"                  { return QUOTE;     }
"`"                  { return BACKQUOTE; }
-?{DIGIT}*"."{DIGIT}+  { yylval.value = atof(yytext); return NUMBER; }
-?{DIGIT}+             { yylval.value = atof(yytext); return NUMBER; }
{SYMBOL}+            { yylval.text = (char*)strdup(yytext); return SYMBOL; }
%%

int yywrap() { return 1; }


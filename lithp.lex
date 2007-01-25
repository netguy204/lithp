%{
/*  This file is part of Lithp.

    Lithp is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Lithp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

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


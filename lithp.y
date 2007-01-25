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

#include "cons.h"
#include <stdio.h>
#define YYDEBUG 1
extern int yylineno;
extern char* yytext;

int yylex();
int yyparse();

extern Atom* program_atom;
void yyerror(char* s);
%}

%union {
   char* text;
   double value;
   Cons* cons;
   Atom* atom;
}

%token O_PARENTH C_PARENTH DOT COMMA QUOTE BACKQUOTE 
%token <value> NUMBER 
%token <text> SYMBOL

%type <atom> sequence list expr

%start program

%%
program: sequence
{
   //PRINT_CONS($1->value.cons);
   program_atom = $1;
}

expr: SYMBOL
{
   $$ = NEW_SYMBOL_ATOM($1);
}
 | NUMBER
{
   $$ = NEW_NUMBER_ATOM($1);
}
 | list
{
   //$$ = NEW_CONS_ATOM(CONS($1, NIL));
   $$ = $1;
}

list: O_PARENTH sequence C_PARENTH
{
   $$ = $2;
}

sequence: 
{
   $$ = NIL;
}
 | expr sequence
{
   $$ = NEW_CONS_ATOM(CONS($1, $2));
}

%%

void yyerror(char* s)
{ 
   fprintf(stderr, "something goofed: %s",s); 
}



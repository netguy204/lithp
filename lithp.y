%{
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



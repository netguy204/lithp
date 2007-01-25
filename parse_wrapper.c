#include <stdio.h>
#include "cons.h"
#include "y.tab.h"

Atom* program_atom; // global that gets the parser return value
extern FILE* yyin;

Atom* PARSE_FILE(FILE* file)
{
   yyin = file;
   yyparse();
   return program_atom;
}


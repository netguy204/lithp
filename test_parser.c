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
    along with lithp; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include "environment.h"
#include "cons.h"
#include "parse_wrapper.h"
#include "lithp_eval.h"
#include "lithp_primitives.h"

int main(int argc, char* argv[])
{
   FILE* infile;
   if(argc == 2) {
      infile = fopen(argv[1],"r");
   } else {
      infile = stdin;
   }
   
   Cons* global_env = newEnvironment(0);
   initializePrimitives(global_env);

   // bootstrap the interpreter
   FILE* bootstrap = fopen("bootstrap.l","r");
   Atom* program = PARSE_FILE(bootstrap);
   fclose(bootstrap);

   // wrap the bootstrap program in seq
   program = NEW_CONS_ATOM2(NEW_SYMBOL_ATOM("seq"), program);

   DEBUG1("bootstrap program: %s", program);

   Atom* bs_result = lithp2_eval(global_env, program);
   DEBUG1("bootstrap result: %s", bs_result);

   // run the user's file
   program = PARSE_FILE(infile);
   if(!ISCONS(program)) {
      ERROR("parser did not return a cons--dunno what to do",0);
      return 0;
   }
     
   DEBUG1("user program: %s", program);
 
   Cons* program_cons = program->value.cons;

   do {
      PRINTF1("Evaluating: %s", CAR(program_cons));
      PRINTF1("Result: %s", lithp2_apply(global_env, CAR(program_cons)));
   } while (CDR(program_cons) && (program_cons = CDRCONS(program_cons)));
   
   fclose(infile);
   
   return 0;
}

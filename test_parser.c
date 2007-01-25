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

   lithp_seq(global_env, program);
   PRINT_CONS(global_env); printf("\n");

   // run the user's file
   program = PARSE_FILE(infile);
   PRINT_ATOM(program); printf("\n");
 
   if(ATOMTYPE(program) != CONS_ATOM) {
      ERROR("parser did not return a cons--dunno what to do",0);
      return 0;
   }
     
   Cons* program_cons = program->value.cons;

   do {
      if(ATOMTYPE(program) != CONS_ATOM) {
         ERROR("parser did not return a cons--dunno what to do",0);
         return 0;
      }
      
      PRINT_ATOM(CAR(program_cons)); printf("\n");
      PRINT_ATOM(lithp_eval_atom(global_env, CAR(program_cons))); printf("\n");
      printf("\n");
   } while (CDR(program_cons) && (program_cons = CDRCONS(program_cons)));
   
   fclose(infile);
   
   return 0;
}

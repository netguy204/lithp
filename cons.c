#include <stdio.h>
#ifndef MACOSX
	#include <malloc.h>
#else
	#include <malloc/malloc.h>
#endif

#include "cons.h"

// create a new Cons cell
Cons* CONS(Atom* car, Atom* cdr)
{
   Cons* cons = (Cons*)malloc(sizeof(Cons));
   CAR(cons) = car;
   CDR(cons) = cdr;
   return cons;
}

Atom* newAtom()
{
   return (Atom*)malloc(sizeof(Atom));
}

Atom* NEW_SYMBOL_ATOM(SYMBOL_TYPE symbol)
{
   Atom* atom = newAtom();
   atom->type = SYMBOL_ATOM;
   atom->value.symbol = symbol;

   return atom;
}

Atom* NEW_NUMBER_ATOM(NUMBER_TYPE number)
{
   Atom* atom = newAtom();
   atom->type = NUMBER_ATOM;
   atom->value.number = number;

   return atom;
}

Atom* NEW_CONS_ATOM(Cons* cons)
{
   Atom* atom = newAtom();
   atom->type = CONS_ATOM;
   atom->value.cons = cons;

   return atom;
}

Atom* NEW_PRIMITIVE_ATOM(LITHP_FUNCTION function)
{
   Atom* atom = newAtom();
   atom->type = PRIMITIVE_ATOM;
   atom->value.function = function;

   return atom;
}

Atom* NEW_LAMBDA_ATOM(Cons* env, Atom* lambda)
{
   Atom* env_atom = NEW_CONS_ATOM(env);
   Cons* lambda_cons = CONS(env_atom, lambda);
   Atom* cons =  NEW_CONS_ATOM(lambda_cons);
   cons->type = LAMBDA_ATOM;
   return cons;
}

Atom* NEW_MACRO_ATOM(Atom* macro)
{
   Atom* macro_a = NEW_CONS_ATOM(macro->value.cons);
   macro_a->type = MACRO_ATOM;
   return macro_a;
}

void PRINT_ATOM(Atom* atom)
{
		if(!atom) {
			printf("nil");
		} else if(ATOMTYPE(atom) == NUMBER_ATOM) {
			printf("%.2f", atom->value.number);
		} else if (ATOMTYPE(atom) == SYMBOL_ATOM) {
			printf("%s", atom->value.symbol);
		} else if (ATOMTYPE(atom) == CONS_ATOM) {
			printf("(");
			PRINT_CONS(atom->value.cons);
			printf(")");
		} else if (ATOMTYPE(atom) == PRIMITIVE_ATOM) {
         printf("#sys-func#");
      } else if (ATOMTYPE(atom) == LAMBDA_ATOM) {
         printf("#lambda#");
         PRINT_CONS(CDRCONS(atom->value.cons));
      } else if (ATOMTYPE(atom) == MACRO_ATOM) {
         printf("#macro#");
         PRINT_CONS(atom->value.cons);
      } else {
			printf("!error!");
		}
}

void PRINT_CONS(Cons* cons)
{
   if(!cons) {
      printf("null_cons");
      return;
   }

	do {
		PRINT_ATOM(CAR(cons));
		if(CDR(cons)) printf(" ");
	} while (CDR(cons) && (CDRTYPE(cons) == CONS_ATOM) && (cons = CDRCONS(cons)));
	
	if(CDR(cons)) {
		printf(". "); PRINT_ATOM(CDR(cons));
	}
}

int EXECUTABLE_ATOM(Atom* atom)
{
   if(!atom) return 0;
   return ((ATOMTYPE(atom) == LAMBDA_ATOM) || 
          (ATOMTYPE(atom) == PRIMITIVE_ATOM));
}


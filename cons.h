#ifndef CONS_H
#define CONS_H

#include <stdio.h>

#define NIL 0
#define ERROR(x,y) printf(x,y); printf("\n");

#define ATOMTYPE(x) (x->type)
#define CAR(x) (x->car)
#define CDR(x) (x->cdr)
#define CARCONS(x) (CAR(x)->value.cons)
#define CDRCONS(x) (CDR(x)->value.cons)
#define CARATOM(x) (CAR(x->value.cons))
#define CDRATOM(x) (CDR(x->value.cons))
#define CARTYPE(x) (ATOMTYPE(CAR(x)))
#define CDRTYPE(x) (ATOMTYPE(CDR(x)))

struct Cons;
struct Atom;
enum AtomType;

typedef char* SYMBOL_TYPE;
typedef double NUMBER_TYPE;
typedef struct Atom Atom;
typedef struct Cons Cons;

typedef Atom*(*LITHP_FUNCTION)(Cons*, Atom*);

typedef enum AtomType AtomType;

enum AtomType {
  LAMBDA_ATOM,
  PRIMITIVE_ATOM,
  SYMBOL_ATOM,
  NUMBER_ATOM,
  CONS_ATOM,
  MACRO_ATOM
};

struct Cons {
  Atom* car;
  Atom* cdr;
};

struct Atom {
  AtomType type;
	union {
      SYMBOL_TYPE symbol;
		NUMBER_TYPE number;
		Cons* cons;
      LITHP_FUNCTION function;
	} value;
};

Cons* CONS(Atom* car, Atom* cdr);
Atom* NEW_SYMBOL_ATOM(SYMBOL_TYPE symbol);
Atom* NEW_NUMBER_ATOM(NUMBER_TYPE number);
Atom* NEW_PRIMITIVE_ATOM(LITHP_FUNCTION function);
Atom* NEW_CONS_ATOM(Cons* cons);
Atom* NEW_LAMBDA_ATOM(Cons* env, Atom* lambda);
Atom* NEW_MACRO_ATOM(Atom* macro);
int EXECUTABLE_ATOM(Atom* atom);
void PRINT_ATOM(Atom* atom);
void PRINT_CONS(Cons* cons);

#endif


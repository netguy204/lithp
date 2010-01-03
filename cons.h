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

#ifndef CONS_H
#define CONS_H

#include <stdio.h>

#define NIL 0
#define ERROR(x,y) printf(x,y); printf("\n");

#define ATOMTYPE(x) (x->type)
#define CAR(x) (x->car)
#define CDR(x) (x->cdr)
#define ASCONS(x) (x->value.cons)
#define ASSYMBOL(x) (x->value.symbol)
#define CARCONS(x) (ASCONS(CAR(x)))
#define CDRCONS(x) (ASCONS(CDR(x)))
#define CARATOM(x) (CAR(ASCONS(x)))
#define CDRATOM(x) (CDR(ASCONS(x)))
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
Atom* NEW_CONS_ATOM2(Atom* car, Atom* cdr);
Atom* NEW_LAMBDA_ATOM(Cons* env, Atom* lambda);
Atom* NEW_MACRO_ATOM(Atom* macro);
int EXECUTABLE_ATOM(Atom* atom);
void PRINT_ATOM(Atom* atom);
void PRINT_CONS(Cons* cons);

#endif


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

/* disable debug output */
/*#define NO_PRINT_DEBUG*/

#include <stdio.h>
#include <stdlib.h>

#define NIL 0

#define APPENDF(buf, buflen, ...) do { buf = appendf(buf, buflen, __VA_ARGS__); } while(0)

#define ERROR(x,...) do { fprintf(stderr, x "\n", __VA_ARGS__); } while(0)
#define ERROR1(x, atom, ...) \
do { \
	char* tmp = PRINT_ATOM(atom); \
	fprintf(stderr, x "\n", tmp __VA_ARGS__); \
	free(tmp); } while(0)

#ifndef NO_PRINT_DEBUG
#define DEBUG(x,...) do { fprintf(stdout, x "\n" __VA_ARGS__); } while(0)
#define DEBUG1(x,atom,...) \
do { \
	char* tmp; \
	fprintf(stdout, x "\n", tmp=PRINT_ATOM(atom) __VA_ARGS__); \
	free(tmp); } \
while(0)
#else
#define DEBUG(x,...) do { } while(0)
#define DEBUG1(x,...) do { } while(0)
#endif

#define PRINTF(x,...) do { fprintf(stdout, x "\n" __VA_ARGS__); } while(0)
#define PRINTF1(x,atom,...) \
do { \
	char* tmp; \
	fprintf(stdout, x "\n", tmp=PRINT_ATOM(atom) __VA_ARGS__); \
	free(tmp); } \
while(0)

#define ATOMTYPE(x) (x->type)
#define ISCONS(x) (ATOMTYPE(x) == CONS_ATOM)
#define ISSYMBOL(x) (ATOMTYPE(x) == SYMBOL_ATOM)
#define ISMACRO(x) (ATOMTYPE(x) == MACRO_ATOM)
#define ISSPECIALPRIM(x) (ATOMTYPE(x) == SPECIAL_PRIMITIVE_ATOM)
#define ISPRIM(x) (ATOMTYPE(x) == PRIMITIVE_ATOM || ISSPECIALPRIM(x))
#define ISSPECIALLAMBDA(x) (ATOMTYPE(x) == SPECIAL_LAMBDA_ATOM)
#define ISLAMBDA(x) (ATOMTYPE(x) == LAMBDA_ATOM || ISSPECIALLAMBDA(x))
#define ISNUMBER(x) (ATOMTYPE(x) == NUMBER_ATOM)

#define ASCONS(x) (x->value.cons)
#define ASSYMBOL(x) (x->value.symbol)
#define ASNUMBER(x) (x->value.number)
#define ASPRIM(x) (x->value.function)

#define CAR(x) (x->car)
#define CDR(x) (x->cdr)
#define CARCONS(x) (ASCONS(CAR(x)))
#define CDRCONS(x) (ASCONS(CDR(x)))
#define CARATOM(x) (CAR(ASCONS(x)))
#define CDRATOM(x) (CDR(ASCONS(x)))
#define CARTYPE(x) (ATOMTYPE(CAR(x)))
#define CDRTYPE(x) (ATOMTYPE(CDR(x)))

#define SPECIAL 1
#define NOT_SPECIAL 0

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
	SPECIAL_LAMBDA_ATOM,
	LAMBDA_ATOM,
	PRIMITIVE_ATOM,
	SPECIAL_PRIMITIVE_ATOM,
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
Atom* NEW_PRIMITIVE_ATOM(LITHP_FUNCTION function, int special);
Atom* NEW_CONS_ATOM(Cons* cons);
Atom* NEW_CONS_ATOM2(Atom* car, Atom* cdr);
Atom* NEW_LAMBDA_ATOM(Cons* env, Atom* lambda, int special);
Atom* NEW_MACRO_ATOM(Atom* macro);
int EXECUTABLE_ATOM(Atom* atom);

char* appendf(char* str, int* bfsz, char* format, ...);
char* PRINT_ATOM(Atom* atom);
char* PRINT_CONS(Atom* cons);

#endif


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
#include <assert.h>
#include <stdarg.h>
#include <string.h>

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

Atom* NEW_CONS_ATOM2(Atom* car, Atom* cdr)
{
	return NEW_CONS_ATOM(CONS(car, cdr));
}

Atom* NEW_PRIMITIVE_ATOM(LITHP_FUNCTION function, int special)
{
	Atom* atom = newAtom();

	if(special) {
		atom->type = SPECIAL_PRIMITIVE_ATOM;
	} else {
		atom->type = PRIMITIVE_ATOM;
	}

	atom->value.function = function;

	return atom;
}

Atom* NEW_LAMBDA_ATOM(Cons* env, Atom* lambda, int special)
{
	Atom* cons =  NEW_CONS_ATOM2(NEW_CONS_ATOM(env), lambda);

	if(special) {
		cons->type = SPECIAL_LAMBDA_ATOM;
	} else {
		cons->type = LAMBDA_ATOM;
	}

	return cons;
}

Atom* NEW_MACRO_ATOM(Atom* macro)
{
	Atom* macro_a = NEW_CONS_ATOM(macro->value.cons);
	macro_a->type = MACRO_ATOM;
	return macro_a;
}

char* appendf(char* str, int* bfsz, char* format, ...) {
	va_list args;

	va_start(args, format);

	int offset = strlen(str);
	int len = vsnprintf(&str[offset], *bfsz, format, args);

	if(len >= *bfsz) {
		// try again...
		str = (char*)realloc(str, len*2);
		*bfsz = len*2;
		vsnprintf(&str[offset], *bfsz, format, args);
	}

	va_end(args);

	return str;
}

char* PRINT_CONS(Atom* atom)
{
	assert(ISCONS(atom));

	// build our buffer
	int buflen = 1024;
	char* buffer = (char*)malloc(buflen);
	buffer[0] = '\0';

	char* tmp;

	APPENDF(buffer, &buflen, "(");
	while(atom) {
		APPENDF(buffer, &buflen, tmp=PRINT_ATOM(CARATOM(atom)));
		free(tmp);

		Atom* next = CDRATOM(atom);
		if(next && !ISCONS(next)) {
			APPENDF(buffer, &buflen, " . ");
			APPENDF(buffer, &buflen, tmp=PRINT_ATOM(next));
			free(tmp);
			break;
		}

		if(next) {
			APPENDF(buffer, &buflen, " ");
		}

		atom = next;
	}
	APPENDF(buffer, &buflen, ")");
	return buffer;
}

char* PRINT_ATOM(Atom* atom)
{
	// build our buffer
	int buflen = 1024;
	char* buffer = (char*)malloc(buflen);
	buffer[0] = '\0';

	if(!atom) {
		APPENDF(buffer, &buflen, "nil");
		return buffer;
	}

	char* tmp;
	switch(ATOMTYPE(atom)) {
		case NUMBER_ATOM: APPENDF(buffer, &buflen, "%.2f", ASNUMBER(atom)); break;
		case SYMBOL_ATOM: APPENDF(buffer, &buflen, "%s", ASSYMBOL(atom)); break;
		case CONS_ATOM: APPENDF(buffer, &buflen, tmp=PRINT_CONS(atom)); free(tmp); break;
		case SPECIAL_PRIMITIVE_ATOM: APPENDF(buffer, &buflen, "<special primitive>"); break;
		case PRIMITIVE_ATOM: APPENDF(buffer, &buflen, "<primitive>"); break;
		case LAMBDA_ATOM: APPENDF(buffer, &buflen, "<lambda>"); break;
		case SPECIAL_LAMBDA_ATOM: APPENDF(buffer, &buflen, "<special lambda>"); break;
		case MACRO_ATOM: APPENDF(buffer, &buflen, "<macro>"); break;
		default: APPENDF(buffer, &buflen, "<?>"); break;
	}

	return buffer;
}

int EXECUTABLE_ATOM(Atom* atom)
{
	if(!atom) return 0;
	return (ISLAMBDA(atom) || ISPRIM(atom) || ISMACRO(atom));
}


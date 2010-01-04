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

#include "lithp_primitives.h"
#include "cons.h"
#include "environment.h"
#include "lithp_eval.h"

Atom* GLOBAL_TRUE;
Atom* GLOBAL_FALSE;

Atom* lithp_lambda(Cons* env, Atom* atom)
{
	// store the environment and the code in new atom
	Atom* lambda = NEW_LAMBDA_ATOM(env, atom, NOT_SPECIAL);
	return lambda;
}

Atom* lithp_macro(Cons* env, Atom* atom)
{
	return NEW_MACRO_ATOM(atom);
}

/* special function */
Atom* lithp_set(Cons* env, Atom* atom)
{
	// evaluate the atom and store it as a new variable
	Atom* symbol_a = CARATOM(atom);
	if(!ISSYMBOL(symbol_a)) {
		ERROR("first argument to set must be a symbol",0);
		return NIL;
	}

	Atom* value = lithp2_eval(env, CARATOM(CDRATOM(atom)));
	newVariable(env, ASSYMBOL(symbol_a), value);
	return value;
}

/* special function */
Atom* lithp_cond(Cons* env, Atom* atom)
{
	Atom* conditional = CARATOM(atom);
	Atom* if_true = CARATOM(CDRATOM(atom));
	Atom* if_false = (CDRATOM(CDRATOM(atom))) ? CARATOM(CDRATOM(CDRATOM(atom))) : NIL;

	conditional = lithp2_eval(env, conditional);

	if(!conditional || conditional == GLOBAL_FALSE) {
		return lithp2_eval(env, if_false);
	} else {
		return lithp2_eval(env, if_true);
	}
}

Atom* lithp_equals(Cons* env, Atom* atom)
{
	Atom* lhs = CARATOM(atom);
	Atom* rhs = CARATOM(CDRATOM(atom));

	if(lhs == rhs) {
		// literally the same
		return GLOBAL_TRUE;
	} else if (ISNUMBER(lhs) && ISNUMBER(rhs) && lhs->value.number == rhs->value.number) {
		// numbers are the same
		return GLOBAL_TRUE;
	} else if (ISCONS(lhs) && ISCONS(rhs)) {
		Atom* next_lhs = lhs;
		Atom* next_rhs = rhs;
		while(next_lhs && next_rhs) {
			if(lithp_equals(env, NEW_CONS_ATOM2(next_lhs, NEW_CONS_ATOM2(next_rhs, 0))) == GLOBAL_FALSE) {
				return GLOBAL_FALSE;
			}

			next_lhs = CDRATOM(next_lhs);
			next_rhs = CDRATOM(next_rhs);
		}

		// make sure we reached the end at the same time
		if(!next_lhs && !next_rhs) {
			return GLOBAL_TRUE;
		} else {
			return GLOBAL_FALSE;
		}
	} else {
		return GLOBAL_FALSE;
	}
}

Atom* lithp_car(Cons* env, Atom* atom)
{
	Atom* arg1 = CARATOM(atom);
	DEBUG1("lithp_car: arg1=%s", arg1);

	if(!arg1 || !ISCONS(arg1)) {
		ERROR("CAR: argument error. 1st arg nil or non-cons","");
		return NIL;
	}

	return CARATOM(arg1);
}

Atom* lithp_cdr(Cons* env, Atom* atom)
{
	Atom* arg1 = CARATOM(atom);

	DEBUG1("lithp_cdr: arg1=%s", arg1);

	if(!arg1 || !ISCONS(arg1)) {
		ERROR("CDR: argument error. 1st arg nil or non-cons","");
		return NIL;
	}

	return CDRATOM(arg1);
}

Atom* lithp_cons(Cons* env, Atom* atom)
{
	Atom* car = CARATOM(atom);
	Atom* cdr = CARATOM(CDRATOM(atom));
	return NEW_CONS_ATOM2(car, cdr);
}

/* special */
Atom* lithp_quote(Cons* env, Atom* atom)
{
	return CARATOM(atom);
}

Atom* lithp_add(Cons* env, Atom* atom)
{
	NUMBER_TYPE value = 0;
	while(atom) {
		Atom* num = CARATOM(atom);
		if(!num || !ISNUMBER(num)) {
			ERROR("you can only add numbers", 0);
			return NIL;
		}
		value += ASNUMBER(num);
		atom = CDRATOM(atom);
	}

	return NEW_NUMBER_ATOM(value);
}

Atom* lithp_mult(Cons* env, Atom* atom)
{
	double value = 1.0;
	while(atom) {
		Atom* num = CARATOM(atom);
		if(!num || !ISNUMBER(num)) {
			ERROR("you can only multiply numbers, fool!",0);
			return NIL;
		}

		value *= ASNUMBER(num);
		atom = CDRATOM(atom);
	}

	return NEW_NUMBER_ATOM(value);
}

/* special function */
Atom* lithp_seq(Cons* env, Atom* atom)
{
	Atom* last_eval = NIL;

	while(atom) {
		DEBUG1("seq executing: %s", atom);
		last_eval = lithp2_eval(env, CARATOM(atom));
		atom = CDRATOM(atom);
	}

	return last_eval;
}

Atom* lithp_print(Cons* env, Atom* atom)
{
	char* tmp = PRINT_ATOM(atom);
	fprintf(stdout, "%s\n", tmp);
	free(tmp);
	return NIL;
}

void initializePrimitives(Cons* env)
{
	GLOBAL_FALSE = NEW_SYMBOL_ATOM("#f");
	GLOBAL_TRUE = NEW_SYMBOL_ATOM("#t");

	REGISTER_PRIMITIVE(env, "+", lithp_add, NOT_SPECIAL);
	REGISTER_PRIMITIVE(env, "*", lithp_mult, NOT_SPECIAL);
	REGISTER_PRIMITIVE(env, "set", lithp_set, SPECIAL);
	REGISTER_PRIMITIVE(env, "lambda", lithp_lambda, SPECIAL);
	REGISTER_PRIMITIVE(env, "car", lithp_car, NOT_SPECIAL);
	REGISTER_PRIMITIVE(env, "cdr", lithp_cdr, NOT_SPECIAL);
	REGISTER_PRIMITIVE(env, "quote", lithp_quote, SPECIAL);
	REGISTER_PRIMITIVE(env, "macro", lithp_macro, SPECIAL);
	REGISTER_PRIMITIVE(env, "cond", lithp_cond, SPECIAL);
	REGISTER_PRIMITIVE(env, "=", lithp_equals, NOT_SPECIAL);
	REGISTER_PRIMITIVE(env, "cons", lithp_cons, NOT_SPECIAL);
	REGISTER_PRIMITIVE(env, "seq", lithp_seq, SPECIAL);
	REGISTER_PRIMITIVE(env, "print", lithp_print, NOT_SPECIAL);
}

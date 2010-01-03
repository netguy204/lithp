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

#include "lithp_eval.h"
#include "cons.h"
#include "environment.h"

int isSpecialFunction(Atom* atom) {
	return 0;
}

Atom* lithp2_eval(Cons* env, Atom* list)
{
	// sanity check
	if(!list || ATOMTYPE(list) != CONS_ATOM) {
		ERROR("atom is not a list atom", "");
		return NIL;
	}

	// what we do depends on what the head is
	Atom* head = CARATOM(list);
	
	// since we want to support evaluatable expressions
	// as the heads of lists, we need to see if the head
	// is a cons. If so, we evaluate that and make our decision
	// based on it's result
	if(ATOMTYPE(head) == CONS_ATOM) {
		head = lithp2_eval(env, head);
	} else if(ATOMTYPE(head) == SYMBOL_ATOM) {
		head = lookupVariable(env, ASSYMBOL(head));
	}

	Atom* new_list = list;

	// now the classic test to see if this is special or not
	if(!isSpecialFunction(head)) {
		// eval each argument, build a new list for apply
		new_list = NEW_CONS_ATOM2(head, 0);
		Atom* last_filled_cons = new_list;
		Atom* arg = CDRATOM(list);
		while(arg) {
			Atom* new_arg = lithp2_eval(env, arg);

			// add the newly eval'd arg to the new list for apply
			Atom* new_cons = NEW_CONS_ATOM2(new_arg, 0);
			CDRATOM(last_filled_cons) = new_cons;
			last_filled_cons = new_cons;

			// move to the next argument
			arg = CDRATOM(arg);
		}
	}

	// apply!
	lithp2_apply(env, list);
}

Atom* lithp2_apply(Cons* env, Atom* list)
{
}

Atom* lithp2_apply_lambda(Cons* env, Atom* list)
{
	Atom* lambda = CARATOM(list);

	// structure of a lambda:
	// (creation_env arg_list . (lambda_body . nil))
	Atom* parent_env = CARATOM(lambda);
	Atom* variable_list = CARATOM(CDRATOM(lambda));
	Atom* value_list = CDRATOM(list);
	Atom* lambda_body = CAR(CDRCONS(CDRCONS(lambda->value.cons)));

	// build new environment
	Cons* lambda_env = newEnvironment(ASCONS(parent_env));
	Atom* next_var_name = variable_list;
	Atom* next_val = value_list;
	while(next_var_name && next_val) {
		Atom* var_name = CARATOM(next_var_name);
		Atom* var_val = CARATOM(next_val);

		// sanity check
		if(ATOMTYPE(var_name) != SYMBOL_ATOM) {
			ERROR("the lambda parameter list must contain only symbols","");
			return NIL;
		}

		newVariable(lambda_env, ASSYMBOL(var_name), var_val);

		next_var_name = CDRATOM(next_var_name);
		next_val = CDRATOM(next_val);
	}

	Atom* result = lithp_eval_atom(lambda_env, lambda_body);
	return result;
}

Atom* lithp_eval_list(Cons* env, Atom* list)
{
	// is this really a list?
	if(!list || ATOMTYPE(list) != CONS_ATOM) {
		ERROR("atom is not a list atom", "");
		return NIL;
	}

	// build a temporary head cons so that we can maim it later
	list = NEW_CONS_ATOM(CONS(CARATOM(list),CDRATOM(list)));

	// FIXME: first lookup symbol, then expand macro, then evaluate list
	if(ATOMTYPE(CARATOM(list)) == SYMBOL_ATOM || ATOMTYPE(CARATOM(list)) == MACRO_ATOM) {
		if(ATOMTYPE(CARATOM(list)) == SYMBOL_ATOM) {
			Atom* symbol_a = lookupVariable(env, CARATOM(list)->value.symbol);

			if(symbol_a && ATOMTYPE(symbol_a) == MACRO_ATOM) {
				CARATOM(list) = symbol_a;
			}
		}

		if(ATOMTYPE(CARATOM(list)) == MACRO_ATOM) {
			Atom* macro = lithp_expand_macro(env, list);
			return lithp_eval_atom(env, macro);
		}
	}

	// what's at the head of the list?
	if(ATOMTYPE(CARATOM(list)) == CONS_ATOM) {
		// execute the list at the head of the list
		//printf("evaluating: "); PRINT_ATOM(CARATOM(list)); printf("\n");
		CARATOM(list) = lithp_eval_list(env, CARATOM(list));
		//printf("done evaluating\n");

		return CARATOM(list); // i think i always want to return newly evaluated lists

		/*
		   if(!EXECUTABLE_ATOM(CARATOM(list))) {
		//ERROR("the list at the head of the list did not return a special "\
		//      "function. returning list", 0);
		return CARATOM(list); // dunno if this is proper behavior
		}
		*/
	} else if(ATOMTYPE(CARATOM(list)) == SYMBOL_ATOM) {
		// look up the symbol at the head and substitute the result
		char* symbol = CARATOM(list)->value.symbol;
		CARATOM(list) = lookupVariable(env, symbol);

		if(!EXECUTABLE_ATOM(CARATOM(list))) {
			return CARATOM(list);
		}
	}

	if(!EXECUTABLE_ATOM(CARATOM(list))) {
		ERROR("something really goofed. we shouldn't be here.",0);
		//PRINT_ATOM(list); printf("\n");
		return NIL; // FIXME: debatable
	}

	// now we've decoded the head of the list--dispatch the call appropriately
	if(ATOMTYPE(CARATOM(list)) == PRIMITIVE_ATOM) {
		return CARATOM(list)->value.function(env, CDRATOM(list));
	} else if(ATOMTYPE(CARATOM(list)) == LAMBDA_ATOM) {
		Atom* result = lithp2_apply_lambda(env, list);
	} else {
		ERROR("programming error. give a coder a hug.", 0);
		return NIL;
	}
}

Atom* lithp_eval_atom(Cons* env, Atom* atom)
{
	if(!atom) {
		return NIL;
	} else if(ATOMTYPE(atom) == NUMBER_ATOM) {
		return atom; // numbers are self evaluating
	} else if(ATOMTYPE(atom) == SYMBOL_ATOM) {
		return lookupVariable(env, atom->value.symbol); // symbols get looked up
	} else if(ATOMTYPE(atom) == CONS_ATOM) {
		return lithp_eval_list(env, atom); // lists get evaluated
	} else {
		ERROR("can't do anything with this atom, returning it",0);
		return atom;
	}
}


Atom* lithp_expand_macro(Cons* env, Atom* list)
{
	Atom* macro = CARATOM(list);

	Cons* variable_list = CARCONS(macro->value.cons);
	Cons* value_list = CDRCONS(list->value.cons);
	Atom* macro_body = CAR(CDRCONS(macro->value.cons));

	// build new environment
	Cons* macro_env = newEnvironment(0);
	do {
		Atom* symbol_a = CAR(variable_list);
		Atom* value = CAR(value_list);

		if(!symbol_a || ATOMTYPE(symbol_a) != SYMBOL_ATOM) {
			ERROR("the macro parameter list must contain only symbols","");
			return NIL;
		}

		SYMBOL_TYPE symbol = symbol_a->value.symbol;
		newVariable(macro_env, symbol, value);
	} while(CDR(variable_list) && CDR(value_list) 
			&& (variable_list = CDRCONS(variable_list))
			&& (value_list = CDRCONS(value_list)));

	return lithp_weak_eval_atom(macro_env, macro_body);
}

Atom* lithp_weak_eval_atom(Cons* env, Atom* atom)
{
	if(!atom) {
		return NIL;
	} else if (ATOMTYPE(atom) == CONS_ATOM) {
		return lithp_weak_eval_list(env, atom);
	} else if (ATOMTYPE(atom) == SYMBOL_ATOM) {
		SYMBOL_TYPE symbol = atom->value.symbol;
		Atom* symbol_a = lookupVariable(env, symbol);

		if(symbol_a) return symbol_a;
		else return atom;
	} else {
		return atom;
	}
}

Atom* lithp_weak_eval_list(Cons* env, Atom* list)
{
	if(!list || ATOMTYPE(list) != CONS_ATOM) {
		ERROR("lithp_weak_eval_list called with non-cons atom","");
		return NIL;
	}

	Cons* old_list = list->value.cons;
	Cons* new_list = CONS(0,0);
	Cons* new_list_head = new_list;

	do {
		CAR(new_list) = lithp_weak_eval_atom(env, CAR(old_list));

		if(CDR(old_list)) {
			CDR(new_list) = NEW_CONS_ATOM(CONS(0,0));
			new_list = CDRCONS(new_list);
		}
	} while (CDR(old_list) && (old_list = CDRCONS(old_list)));

	return NEW_CONS_ATOM(new_list_head);
}

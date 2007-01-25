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

#include "lithp_primitives.h"
#include "cons.h"
#include "environment.h"
#include "lithp_eval.h"

Atom* GLOBAL_TRUE;
Atom* GLOBAL_FALSE;

Atom* lithp_lambda(Cons* env, Atom* atom)
{
   // store the environment and the code in new atom
   Atom* lambda = NEW_LAMBDA_ATOM(env, atom);
   return lambda;
}

Atom* lithp_macro(Cons* env, Atom* atom)
{
   return NEW_MACRO_ATOM(atom);
}

Atom* lithp_set(Cons* env, Atom* atom)
{
   // evaluate the atom and store it as a new variable
   Atom* symbol_a = CARATOM(atom);
   if(ATOMTYPE(symbol_a) != SYMBOL_ATOM) {
      ERROR("first argument to set must be a symbol",0);
      return NIL;
   }
   
   SYMBOL_TYPE symbol = symbol_a->value.symbol;
   Atom* value = lithp_eval_atom(env, CARATOM(CDRATOM(atom)));
   newVariable(env, symbol, value);
   return value;
}

Atom* lithp_cond(Cons* env, Atom* atom)
{
   Atom* conditional = CARATOM(atom);
   Atom* if_true = CARATOM(CDRATOM(atom));
   Atom* if_false = (CDRATOM(CDRATOM(atom))->value.cons) ? CARATOM(CDRATOM(CDRATOM(atom))) : 0;

   conditional = lithp_eval_atom(env, conditional);

   if(!conditional || conditional == GLOBAL_FALSE) {
      if(if_false) {
         return lithp_eval_atom(env, if_false);
      } else {
         return NIL;
      }
   } else {
      return lithp_eval_atom(env, if_true);
   }
}

Atom* lithp_equals(Cons* env, Atom* atom)
{
   Atom* lhs = lithp_eval_atom(env, CARATOM(atom));
   Atom* rhs = lithp_eval_atom(env, CARATOM(CDRATOM(atom)));

   if(lhs == rhs) {
      // literally the same
      return GLOBAL_TRUE;
   } else if (ATOMTYPE(lhs) == NUMBER_ATOM 
              && ATOMTYPE(rhs) == NUMBER_ATOM
              && lhs->value.number == rhs->value.number) {
      // numbers are the same
      return GLOBAL_TRUE;
   } else {
      return GLOBAL_FALSE;
   }
}

Atom* lithp_car(Cons* env, Atom* atom)
{
   //printf("car called with: "); PRINT_ATOM(atom); printf("\n");
   if(atom && ATOMTYPE(atom) == CONS_ATOM) {
      Atom* result = lithp_eval_atom(env, atom);
      if(result && ATOMTYPE(result) == CONS_ATOM) {
         return CARATOM(result);
      } else {
         ERROR("car argument evaluated to non-cons","");
         return NIL;
      }
   } else {
      ERROR("tried to car a non-cons--cryptic, eh?","");
      return NIL;
   }
}

Atom* lithp_cdr(Cons* env, Atom* atom)
{
   //printf("cdr called with: "); PRINT_ATOM(atom); printf("\n");
   if(atom && ATOMTYPE(atom) == CONS_ATOM) {
      Atom* result = lithp_eval_atom(env, atom);
      if(result && ATOMTYPE(result) == CONS_ATOM) {
         return CDRATOM(result);
      } else {
         ERROR("car argument evaluated to non-cons","");
         return NIL;
      }
   } else {
      ERROR("tried to cdr a non-cons--cryptic, eh?","");
      return NIL;
   }
}

Atom* lithp_cons(Cons* env, Atom* atom)
{
   Atom* car = lithp_eval_atom(env, CARATOM(atom));
   Atom* cdr = lithp_eval_atom(env, CARATOM(CDRATOM(atom)));

   return NEW_CONS_ATOM(CONS(car, cdr));
}

Atom* lithp_quote(Cons* env, Atom* atom)
{
   return CARATOM(atom);
}

Atom* lithp_add(Cons* env, Atom* atom)
{
   Atom* temp;
   NUMBER_TYPE value = 0;

   do {
      temp = lithp_eval_atom(env, CARATOM(atom));

      if(!temp || ATOMTYPE(temp) != NUMBER_ATOM) {
         ERROR("you can only add numbers, fool!",0);
         return ;
      }

      value += temp->value.number;
   } while(atom = CDRATOM(atom));

   return NEW_NUMBER_ATOM(value);
}

Atom* lithp_mult(Cons* env, Atom* atom)
{
   Atom* temp;
   double value = 1.0;

   do {
      temp = lithp_eval_atom(env, CARATOM(atom));

      if(!temp || ATOMTYPE(temp) != NUMBER_ATOM) {
         ERROR("you can only multiply numbers, fool!",0);
         PRINT_ATOM(temp);
         return NIL;
      }

      value *= temp->value.number;
   } while(atom = CDRATOM(atom));

   return NEW_NUMBER_ATOM(value);
}

Atom* lithp_seq(Cons* env, Atom* atom)
{
   Cons* seq_cons = atom->value.cons;
   Atom* last_eval = NIL;

   do {
      if(ATOMTYPE(atom) != CONS_ATOM) {
         ERROR("sequence must be a cons--dunno what to do",0);
         return 0;
      }
      
      last_eval = lithp_eval_atom(env, CAR(seq_cons));
   } while (CDR(seq_cons) && (seq_cons = CDRCONS(seq_cons)));

   return last_eval;
}
 
void initializePrimitives(Cons* env)
{
   GLOBAL_FALSE = NEW_SYMBOL_ATOM("#f");
   GLOBAL_TRUE = NEW_SYMBOL_ATOM("#t");

   REGISTER_PRIMITIVE(env, "+", lithp_add);
   REGISTER_PRIMITIVE(env, "*", lithp_mult);
   REGISTER_PRIMITIVE(env, "set", lithp_set);
   REGISTER_PRIMITIVE(env, "lambda", lithp_lambda);
   REGISTER_PRIMITIVE(env, "car", lithp_car);
   REGISTER_PRIMITIVE(env, "cdr", lithp_cdr);
   REGISTER_PRIMITIVE(env, "quote", lithp_quote);
   REGISTER_PRIMITIVE(env, "macro", lithp_macro);
   REGISTER_PRIMITIVE(env, "cond", lithp_cond);
   REGISTER_PRIMITIVE(env, "=", lithp_equals);
   REGISTER_PRIMITIVE(env, "cons", lithp_cons);
   REGISTER_PRIMITIVE(env, "seq", lithp_seq);
}

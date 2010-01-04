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

#include "cons.h"
#include "environment.h"

Cons* newEnvironment(Cons* parent)
{
  return CONS(NEW_CONS_ATOM(parent), NIL);
}

// ( name . (variable))
// env => (parent (name . var) (name . var))
void newVariable(Cons* env, char* name, Atom* atom)
{
   if(!env) {
      ERROR("can't add variable to null environment", "");
      return;
   }
	
   // look for an existing symbol by that name
   while(CDR(env) && (env = CDRCONS(env))) {
      if(strcmp(CAR(CARCONS(env))->value.symbol, name)==0) {
         CDR(CARCONS(env)) = atom; // if we find it already, replace it
         return;
      }
   }
   
   // otherwise add a new one
   Cons* pair = CONS(NEW_SYMBOL_ATOM(name), atom);
   CDR(env) = NEW_CONS_ATOM(CONS(NEW_CONS_ATOM(pair),NIL));
}

Atom* lookupVariable(Cons* env, char* name)
{
   Cons* parent = CARCONS(env);
	
   while(CDR(env) && (env = CDRCONS(env))) {
      Cons* pair = CARCONS(env); // (name . var)
      if(strcmp(CAR(pair)->value.symbol, name) == 0) return CDR(pair);
   }

   if(parent) return lookupVariable(parent, name);
	
   return 0;
}

void REGISTER_PRIMITIVE(Cons* env, char* name, LITHP_FUNCTION function, int special)
{
   newVariable(env, name, NEW_PRIMITIVE_ATOM(function, special));
}



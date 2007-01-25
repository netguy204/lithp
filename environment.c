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

void REGISTER_PRIMITIVE(Cons* env, char* name, LITHP_FUNCTION function)
{
   newVariable(env, name, NEW_PRIMITIVE_ATOM(function));
}



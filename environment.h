#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "cons.h"

void REGISTER_PRIMITIVE(Cons* env, char* name, LITHP_FUNCTION function);
Cons* newEnvironment(Cons* parent);
void newVariable(Cons* env, char* name, Atom* atom);
Atom* lookupVariable(Cons* env, char* name);

#endif


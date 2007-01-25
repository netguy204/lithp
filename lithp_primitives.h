#ifndef LITHP_PRIMITIVES_H
#define LITHP_PRIMITIVES_H

#include "cons.h"

extern Atom* GLOBAL_FALSE;
extern Atom* GLOBAL_TRUE;

Atom* lithp_macro(Cons* env, Atom* atom);
Atom* lithp_lambda(Cons* env, Atom* atom);
Atom* lithp_set(Cons* env, Atom* atom);
Atom* lithp_cond(Cons* env, Atom* atom);
Atom* lithp_equals(Cons* env, Atom* atom);
Atom* lithp_car(Cons* env, Atom* atom);
Atom* lithp_cdr(Cons* env, Atom* atom);
Atom* lithp_quote(Cons* env, Atom* atom);
Atom* lithp_add(Cons* env, Atom* atom);
Atom* lithp_mult(Cons* env, Atom* atom);
Atom* lithp_seq(Cons* env, Atom* atom);
void intializePrimitives(Cons* env);

#endif


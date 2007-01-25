#ifndef LITHP_EVAL_H
#define LITHP_EVAL_H

#include "cons.h"

Atom* lithp_eval_list(Cons* env, Atom* list);
Atom* lithp_eval_atom(Cons* env, Atom* atom);
Atom* lithp_expand_macro(Cons* env, Atom* list);
Atom* lithp_weak_eval_atom(Cons* env, Atom* atom);
Atom* lithp_weak_eval_list(Cons* env, Atom* list);

#endif


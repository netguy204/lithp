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
Atom* lithp_print(Cons* env, Atom* atom);

void intializePrimitives(Cons* env);

#endif


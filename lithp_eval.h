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

#ifndef LITHP_EVAL_H
#define LITHP_EVAL_H

#include "cons.h"

Atom* lithp2_eval(Cons* env, Atom* list);
Atom* lithp2_apply(Cons* env, Atom* list);
Atom* lithp2_apply_lambda(Cons* env, Atom* list);
Atom* lithp_expand_macro(Cons* env, Atom* list);
Atom* lithp_weak_eval_atom(Cons* env, Atom* atom);
Atom* lithp_weak_eval_list(Cons* env, Atom* list);

#endif


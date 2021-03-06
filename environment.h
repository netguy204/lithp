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

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "cons.h"

void REGISTER_PRIMITIVE(Cons* env, char* name, LITHP_FUNCTION function, int special);
Cons* newEnvironment(Cons* parent);
void newVariable(Cons* env, char* name, Atom* atom);
Atom* lookupVariable(Cons* env, char* name);

#endif


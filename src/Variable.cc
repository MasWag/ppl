/* Variable class implementation (non-inline functions).
   Copyright (C) 2001 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#include <config.h>

#include "Variable.defs.hh"
#if OUTLINE
#include "Variable.inlines.hh"
#endif

#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

std::ostream&
PPL::operator <<(std::ostream& s, const Variable& v) {
  unsigned int varid = v.id();
  char c = 'A' + (varid % 26);
  unsigned int i = varid / 26;
  s << c;
  if (i > 0)
    s << i;
  return s;
}

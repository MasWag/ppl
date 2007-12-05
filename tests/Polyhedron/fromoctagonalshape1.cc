/* Test BD_Shape::BD_Shape(const C_Polyhedron&).
   Copyright (C) 2001-2007 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#include "ppl_test.hh"

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A + B <= 5);
  cs.insert(A + B >= -8);
  cs.insert(A >= 0);
  cs.insert(B <= 7);
  cs.insert(A - B <= 10);
  TOctagonal_Shape os(cs);

  C_Polyhedron ph(os);

  C_Polyhedron known_result(cs);

  bool ok = (ph == known_result);

  print_constraints(ph, "*** ph ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
END_MAIN

/* Testing Polyhedron::affine_preimage(): we apply this function to
   a polyhedron defined by its system of constraints.
   Copyright (C) 2001, 2002 Roberto Bagnara <bagnara@cs.unipr.it>

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

#include "ppl_install.hh"
#include "print.hh"
#include "ehandlers.hh"

using namespace std;
using namespace Parma_Polyhedra_Library;

#define NOISY 0

int
main() {
  set_handlers();

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Polyhedron ph(3);
  ph.insert(A + C == 0);
  ph.insert(A + B >= 0);
  ph.insert(A + B - 1 >= 0);

#if NOISY
  cout << "--- ph ---" << endl << ph << endl;
#endif

  ph.affine_preimage(A, A + B);

#if NOISY
  cout << "--- ph after ph.affine_preimage(A, A+B) ---" << endl << ph << endl;
#endif

  Polyhedron known_result(3);
  known_result.insert(A + B + C == 0);
  known_result.insert(A + 2*B >= 0);
  known_result.insert(A + 2*B -1 >= 0);

  int retval = (ph == known_result) ? 0 : 1;

  return retval;
}

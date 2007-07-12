/* Test copy construction of grids.
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

// Universe and empty grids of the first ten dimensions.

bool
test01() {
  bool ok = true;
  for (unsigned int dim = 0; dim < 10; ++dim) {

    nout << "dimension: " << dim << endl;

    // Universe.

    Grid gr(dim);
    print_congruences(gr, "*** gr ***");

    Grid gr_copy = gr;
    print_congruences(gr_copy, "*** gr_copy ***");

    Grid known_gr(dim);

    ok &= (gr_copy == known_gr);
    if (ok) {

      // Empty.

      gr = Grid(dim, EMPTY);
      print_generators(gr, "*** gr ***");

      Grid gr_copy = gr;
      print_generators(gr_copy, "*** gr_copy ***");

      Grid known_gr(dim, EMPTY);

      ok &= (gr_copy == known_gr);
    }
  };

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN

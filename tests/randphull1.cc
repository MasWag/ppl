/* Compute poly-hulls of random polytopes.
   Copyright (C) 2001-2004 Roberto Bagnara <bagnara@cs.unipr.it>

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

#include "ppl_test.hh"

using namespace std;
using namespace Parma_Polyhedra_Library;

#ifndef NOISY
#define NOISY 0
#endif

static int
count_points(const C_Polyhedron& ph) {
  if (ph.is_empty() || ph.space_dimension() == 0)
    return 0;

  int count = 0;
  const Generator_System& gs = ph.generators();
  for (Generator_System::const_iterator i = gs.begin(), gs_end = gs.end();
       i != gs_end;
       ++i)
    if (i->type() == Generator::POINT)
      ++count;
  return count;
}

#if NOISY
#define COUNT(ph) cout << count_points(ph) << endl
#else
#define COUNT(ph) (void) count_points(ph)
#endif

int
main() TRY {
  set_handlers();

  // Set up a random numbers' generator.
  gmp_randclass rg(gmp_randinit_default);

  Variable x(0);
  Variable y(1);
  Variable z(2);

  const int maxc = 10000;
  C_Polyhedron ph(3, C_Polyhedron::EMPTY);
  COUNT(ph);
  for (int n = 1; n <= 200; ++n) {
    const Integer cx = mpz_class(rg.get_z_range(maxc));
    const Integer cy = mpz_class(rg.get_z_range(maxc));
    const Integer cz = mpz_class(rg.get_z_range(maxc));
    ph.add_generator(point(cx*x + cy*y + cz*z));
    COUNT(ph);
  }

  return 0;
}
CATCH

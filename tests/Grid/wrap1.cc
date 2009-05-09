/* Test Grid::wrap_assign().
   Copyright (C) 2001-2009 Roberto Bagnara <bagnara@cs.unipr.it>

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
  Variable x(0);
  Variable y(1);
  Grid gr(2);
  gr.add_congruence((x + 24 %= 8*y) / 2);
  gr.add_congruence((y %= 1) / 3);

  print_congruences(gr, "*** gr ***");

  Variables_Set vars(x, y);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  Grid known_result(2);
  known_result.add_congruence((x + 24 %= 8*y) / 2);
  known_result.add_congruence((y %= 0) / 1);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Grid gr(3);
  gr.add_congruence(x + 24 %= 8*y);
  gr.add_congruence((y %= 1) / 2);

  print_congruences(gr, "*** gr ***");

  Variables_Set vars(x, y);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_UNDEFINED);

  Grid known_result(3);
  known_result.add_congruence(x %= 0);
  known_result.add_congruence(y %= 0);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Grid gr(2);
  gr.add_congruence((x + 24 %= 8*y) / 255);
  gr.add_congruence(x %= 0);

  print_congruences(gr, "*** gr ***");

  Variables_Set vars(x, y);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_IMPOSSIBLE);

  Grid known_result(2);
  known_result.add_congruence(x %= 0);
  known_result.add_congruence((x + 24 %= 8*y) / 255);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr(4);
  gr.add_congruence((x %= 255) / 0);

  print_congruences(gr, "*** gr ***");

  Variables_Set vars(x, w);

  gr.wrap_assign(vars, BITS_8, UNSIGNED, OVERFLOW_WRAPS);

  Grid known_result(4);
  known_result.add_congruence((x %= 255) / 0);

  bool ok = (gr == known_result);

  print_congruences(gr, "*** gr.wrap_assign(...) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
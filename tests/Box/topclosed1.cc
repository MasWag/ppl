/* Test Box::is_topologically_closed().
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2019 BUGSENG srl (http://bugseng.com)

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
site: http://bugseng.com/products/ppl/ . */

#include "ppl_test.hh"

namespace {

/* unbounded 2-dimensional box */
bool
test01() {
  Variable x(0);
  Variable y(1);

  TBox box(3);

  box.add_constraint(x <= 2);
  box.add_constraint(y <= 2);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

/* bounded 5-dimensional box */
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBox box(5);

  box.add_constraint(A <= 5);
  box.add_constraint(A >= 3);
  box.add_constraint(B == 0);
  box.add_constraint(C <= 2);
  box.add_constraint(C >= 1);
  box.add_constraint(D == -7);
  box.add_constraint(E == 1);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

/* 0-dimensional universe box */
bool
test03() {
  TBox box(0);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

/* 2-dimensional empty box */
bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A <= 3);
  box.add_constraint(B <= -5);
  box.add_constraint(A >= 3);
  box.add_constraint(B > -5);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

/* 2-dimensional empty box */
bool
test05() {
  TBox box(2, EMPTY);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

/* 2-dimensional universe box */
bool
test06() {
  TBox box(2);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

/* 4-dimensional topologically open box */
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable D(3);

  TBox box(4);
  box.add_constraint(A < 1);
  box.add_constraint(B <= 7);

  print_constraints(box, "*** box ***");

  bool ok = !box.is_topologically_closed();

  return ok;
}

/* 0-dimensional empty box */
bool
test08() {

  TBox box(0, EMPTY);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A == 0);
  box.add_constraint(B == 2);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TBox box(7);
  box.add_constraint(A <= 1);
  box.add_constraint(B == 2);
  box.add_constraint(A >= 6);

  print_constraints(box, "*** box ***");

  bool ok = box.is_topologically_closed();

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
END_MAIN



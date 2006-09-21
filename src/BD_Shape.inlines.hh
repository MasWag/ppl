/* BD_Shape class implementation: inline functions.
   Copyright (C) 2001-2006 Roberto Bagnara <bagnara@cs.unipr.it>

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
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#ifndef PPL_BD_Shape_inlines_hh
#define PPL_BD_Shape_inlines_hh 1

#include "C_Polyhedron.defs.hh"
#include "Poly_Con_Relation.defs.hh"
#include "Poly_Gen_Relation.defs.hh"
#include <cassert>
#include <vector>
#include <iostream>
#include <algorithm>

namespace Parma_Polyhedra_Library {

namespace Implementation {
namespace BD_Shapes {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Extract the numerator and denominator components of \p from.
/*! \relates Parma_Polyhedra_Library::BD_Shape */
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
template <typename T, typename Policy>
inline void
numer_denom(const Checked_Number<T, Policy>& from,
	    Coefficient& num, Coefficient& den) {
  assert(!is_not_a_number(from)
	 && !is_minus_infinity(from)
	 && !is_plus_infinity(from));
  mpq_class q;
  assign_r(q, from, ROUND_NOT_NEEDED);
  num = q.get_num();
  den = q.get_den();
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Divides \p x by \p y into \p to, rounding the result towards plus infinity.
/*! \relates Parma_Polyhedra_Library::BD_Shape */
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
template <typename T, typename Policy>
inline void
div_round_up(Checked_Number<T, Policy>& to,
	     Coefficient_traits::const_reference x,
	     Coefficient_traits::const_reference y) {
  mpq_class qx;
  mpq_class qy;
  // Note: this code assumes that a Coefficient is always convertible
  // to an mpq_class without loss of precision.
  assign_r(qx, x, ROUND_NOT_NEEDED);
  assign_r(qy, y, ROUND_NOT_NEEDED);
  div_assign_r(qx, qx, qy, ROUND_NOT_NEEDED);
  assign_r(to, qx, ROUND_UP);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Assigns to \p x the minimum between \p x and \p y.
/*! \relates Parma_Polyhedra_Library::BD_Shape */
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
template <typename N>
inline void
min_assign(N& x, const N& y) {
  if (x > y)
    x = y;
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Assigns to \p x the maximum between \p x and \p y.
/*! \relates Parma_Polyhedra_Library::BD_Shape */
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
template <typename N>
inline void
max_assign(N& x, const N& y) {
  if (x < y)
    x = y;
}

} // namespace BD_Shapes
} // namespace Implementation


template <typename T>
inline dimension_type
BD_Shape<T>::max_space_dimension() {
  // One dimension is reserved to have a value of type dimension_type
  // that does not represent a legal dimension.
  return std::min(DB_Matrix<N>::max_num_rows() - 1,
		  DB_Matrix<N>::max_num_columns() - 1);
}

template <typename T>
inline bool
BD_Shape<T>::marked_empty() const {
  return status.test_empty();
}

template <typename T>
inline void
BD_Shape<T>::set_empty() {
  status.set_empty();
  assert(OK());
  assert(marked_empty());
}

template <typename T>
inline void
BD_Shape<T>::set_zero_dim_univ() {
  status.set_zero_dim_univ();
}

template <typename T>
inline bool
BD_Shape<T>::marked_shortest_path_closed() const {
  return status.test_shortest_path_closed();
}

template <typename T>
inline bool
BD_Shape<T>::marked_shortest_path_reduced() const {
  return status.test_shortest_path_reduced();
}

template <typename T>
inline
BD_Shape<T>::BD_Shape(const dimension_type num_dimensions,
		      const Degenerate_Element kind)
  : dbm(num_dimensions + 1), status(), redundancy_dbm() {
  if (kind == EMPTY)
    set_empty();
  else {
    if (num_dimensions > 0)
      // A (non zero-dim) universe BDS is closed.
      status.set_shortest_path_closed();
    assert(OK());
  }
}

template <typename T>
inline
BD_Shape<T>::BD_Shape(const BD_Shape& y)
  : dbm(y.dbm), status(y.status), redundancy_dbm() {
  if (y.marked_shortest_path_reduced())
    redundancy_dbm = y.redundancy_dbm;
}

template <typename T>
template <typename U>
inline
BD_Shape<T>::BD_Shape(const BD_Shape<U>& y)
  : dbm(y.dbm), status(), redundancy_dbm() {
  // TODO: handle flags properly, possibly taking special cases into account.
  if (y.marked_empty())
    set_empty();
  else if (y.status.test_zero_dim_univ())
    set_zero_dim_univ();
}

template <typename T>
inline bool
BD_Shape<T>::add_constraint_and_minimize(const Constraint& c) {
  add_constraint(c);
  shortest_path_closure_assign();
  return !marked_empty();
}

template <typename T>
inline void
BD_Shape<T>::add_constraints(const Constraint_System& cs) {
  for (Constraint_System::const_iterator i = cs.begin(),
	 iend = cs.end(); i != iend; ++i)
    add_constraint(*i);
  assert(OK());
}

template <typename T>
inline bool
BD_Shape<T>::add_constraints_and_minimize(const Constraint_System& cs) {
  add_constraints(cs);
  shortest_path_closure_assign();
  return !marked_empty();
}

template <typename T>
inline
BD_Shape<T>::BD_Shape(const Constraint_System& cs)
  : dbm(cs.space_dimension() + 1), status(), redundancy_dbm() {
  if (cs.space_dimension() > 0)
    // A (non zero-dim) universe BDS is shortest-path closed.
    status.set_shortest_path_closed();
  add_constraints(cs);
}

template <typename T>
inline dimension_type
BD_Shape<T>::affine_dimension() const {
  const dimension_type space_dim = space_dimension();

  // Shortest-path closure is necessary to detect emptiness
  // and all (possibly implicit) equalities.
  shortest_path_closure_assign();
  if (marked_empty())
    return 0;

  // The vector `predecessor' is used to represent equivalence classes:
  // `predecessor[i] == i' if and only if `i' is the leader of its
  // equivalence class (i.e., the minimum index in the class);
  std::vector<dimension_type> predecessor;
  compute_predecessors(predecessor);

  // Due to the fictitious variable `0', the affine dimension is one
  // less the number of equivalence classes.
  dimension_type affine_dim = 0;
  // Note: disregard the first equivalence class.
  for (dimension_type i = 1; i <= space_dim; ++i)
    if (predecessor[i] == i)
      ++affine_dim;

  return affine_dim;
}

template <typename T>
inline BD_Shape<T>&
BD_Shape<T>::operator=(const BD_Shape& y) {
  dbm = y.dbm;
  status = y.status;
  if (y.marked_shortest_path_reduced())
    redundancy_dbm = y.redundancy_dbm;
  return *this;
}

template <typename T>
inline
BD_Shape<T>::~BD_Shape() {
}

template <typename T>
inline void
BD_Shape<T>::swap(BD_Shape& y) {
  std::swap(dbm, y.dbm);
  std::swap(status, y.status);
  std::swap(redundancy_dbm, y.redundancy_dbm);
}

template <typename T>
inline dimension_type
BD_Shape<T>::space_dimension() const {
  return dbm.num_rows() - 1;
}

template <typename T>
inline bool
BD_Shape<T>::is_empty() const {
  shortest_path_closure_assign();
  return marked_empty();
}

/*! \relates BD_Shape */
template <typename T>
inline bool
operator==(const BD_Shape<T>& x, const BD_Shape<T>& y) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim BDSs are equal if and only if they are both empty or universe.
  if (x_space_dim == 0)
    if (x.marked_empty())
      return y.marked_empty();
    else
      return !y.marked_empty();

  // The exact equivalence test requires shortest-path closure.
  x.shortest_path_closure_assign();
  y.shortest_path_closure_assign();

  // If one of two BDSs is empty, then they are equal
  // if and only if the other BDS is empty too.
  if (x.marked_empty())
    return y.marked_empty();
  if (y.marked_empty())
    return false;
  // Check for syntactic equivalence of the two (shortest-path closed)
  // systems of bounded differences.
  return x.dbm == y.dbm;
}

/*! \relates BD_Shape */
template <typename T>
inline bool
operator!=(const BD_Shape<T>& x, const BD_Shape<T>& y) {
  return !(x == y);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const BD_Shape<T>& x,
			    const BD_Shape<T>& y,
			    const Rounding_Dir dir,
			    Temp& tmp0,
			    Temp& tmp1,
			    Temp& tmp2) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim BDSs are equal if and only if they are both empty or universe.
  if (x_space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      r = PLUS_INFINITY;
    return true;
  }

  // The distance computation requires shortest-path closure.
  x.shortest_path_closure_assign();
  y.shortest_path_closure_assign();

  // If one of two BDSs is empty, then they are equal if and only if
  // the other BDS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
   if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      r = PLUS_INFINITY;
   return true;
  }

  return rectilinear_distance_assign(r, x.dbm, y.dbm, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const BD_Shape<T>& x,
			    const BD_Shape<T>& y,
			    const Rounding_Dir dir) {
  static Checked_Number<Temp, Extended_Number_Policy> tmp0;
  static Checked_Number<Temp, Extended_Number_Policy> tmp1;
  static Checked_Number<Temp, Extended_Number_Policy> tmp2;
  return rectilinear_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const BD_Shape<T>& x,
			    const BD_Shape<T>& y,
			    const Rounding_Dir dir) {
  return rectilinear_distance_assign<To, To, T>(r, x, y, dir);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const BD_Shape<T>& x,
			  const BD_Shape<T>& y,
			  const Rounding_Dir dir,
			  Temp& tmp0,
			  Temp& tmp1,
			  Temp& tmp2) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim BDSs are equal if and only if they are both empty or universe.
  if (x_space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      r = PLUS_INFINITY;
    return true;
  }

  // The distance computation requires shortest-path closure.
  x.shortest_path_closure_assign();
  y.shortest_path_closure_assign();

  // If one of two BDSs is empty, then they are equal if and only if
  // the other BDS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
   if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      r = PLUS_INFINITY;
   return true;
  }

  return euclidean_distance_assign(r, x.dbm, y.dbm, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const BD_Shape<T>& x,
			  const BD_Shape<T>& y,
			  const Rounding_Dir dir) {
  static Checked_Number<Temp, Extended_Number_Policy> tmp0;
  static Checked_Number<Temp, Extended_Number_Policy> tmp1;
  static Checked_Number<Temp, Extended_Number_Policy> tmp2;
  return euclidean_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const BD_Shape<T>& x,
			  const BD_Shape<T>& y,
			  const Rounding_Dir dir) {
  return euclidean_distance_assign<To, To, T>(r, x, y, dir);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const BD_Shape<T>& x,
			   const BD_Shape<T>& y,
			   const Rounding_Dir dir,
			   Temp& tmp0,
			   Temp& tmp1,
			   Temp& tmp2) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim BDSs are equal if and only if they are both empty or universe.
  if (x_space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      r = PLUS_INFINITY;
    return true;
  }

  // The distance computation requires shortest-path closure.
  x.shortest_path_closure_assign();
  y.shortest_path_closure_assign();

  // If one of two BDSs is empty, then they are equal if and only if
  // the other BDS is empty too.
  if (x.marked_empty() ||  y.marked_empty()) {
   if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      r = PLUS_INFINITY;
   return true;
  }

  return l_infinity_distance_assign(r, x.dbm, y.dbm, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename Temp, typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const BD_Shape<T>& x,
			   const BD_Shape<T>& y,
			   const Rounding_Dir dir) {
  static Checked_Number<Temp, Extended_Number_Policy> tmp0;
  static Checked_Number<Temp, Extended_Number_Policy> tmp1;
  static Checked_Number<Temp, Extended_Number_Policy> tmp2;
  return l_infinity_distance_assign(r, x, y, dir, tmp0, tmp1, tmp2);
}

/*! \relates BD_Shape */
template <typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const BD_Shape<T>& x,
			   const BD_Shape<T>& y,
			   const Rounding_Dir dir) {
  return l_infinity_distance_assign<To, To, T>(r, x, y, dir);
}

template <typename T>
inline void
BD_Shape<T>::add_dbm_constraint(const dimension_type i,
				const dimension_type j,
				N k) {
  // Private method: the caller has to ensure the following.
  assert(i <= space_dimension() && j <= space_dimension() && i != j);
  N& dbm_ij = dbm[i][j];
  if (dbm_ij > k) {
    dbm_ij = k;
    if (marked_shortest_path_closed())
      status.reset_shortest_path_closed();
  }
}

template <typename T>
inline void
BD_Shape<T>::add_dbm_constraint(const dimension_type i,
				const dimension_type j,
				Coefficient_traits::const_reference num,
				Coefficient_traits::const_reference den) {
  // Private method: the caller has to ensure the following.
  assert(i <= space_dimension() && j <= space_dimension() && i != j);
  assert(den != 0);
  N k;
  Implementation::BD_Shapes::div_round_up(k, num, den);
  add_dbm_constraint(i, j, k);
}

template <typename T>
inline bool
BD_Shape<T>::strictly_contains(const BD_Shape& y) const {
  const BD_Shape<T>& x = *this;
  return x.contains(y) && !y.contains(x);
}

template <typename T>
inline bool
BD_Shape<T>::bds_hull_assign_and_minimize(const BD_Shape& y) {
  bds_hull_assign(y);
  assert(marked_empty()
	 || space_dimension() == 0 || marked_shortest_path_closed());
  return !marked_empty();
}

template <typename T>
inline void
BD_Shape<T>::upper_bound_assign(const BD_Shape& y) {
  bds_hull_assign(y);
}

template <typename T>
inline bool
BD_Shape<T>::bds_hull_assign_if_exact(const BD_Shape&) {
  // TODO: this must be properly implemented.
  return false;
}

template <typename T>
inline bool
BD_Shape<T>::upper_bound_assign_if_exact(const BD_Shape& y) {
  return bds_hull_assign_if_exact(y);
}

template <typename T>
inline void
BD_Shape<T>::difference_assign(const BD_Shape& y) {
  bds_difference_assign(y);
}

template <typename T>
inline void
BD_Shape<T>::remove_higher_space_dimensions(const dimension_type new_dim) {
  // Dimension-compatibility check: the variable having
  // maximum index is the one occurring last in the set.
  if (new_dim > space_dimension())
    throw_dimension_incompatible("remove_higher_space_dimensions(nd)",
				 new_dim);

  // The removal of no dimensions from any BDS is a no-op.
  // Note that this case also captures the only legal removal of
  // dimensions from a zero-dim space BDS.
  if (new_dim == space_dimension()) {
    assert(OK());
    return;
  }

  // Shortest-path closure is necessary as in remove_space_dimensions().
  shortest_path_closure_assign();
  dbm.resize_no_copy(new_dim + 1);

  // Shortest-path closure is maintained.
  // TODO: see whether or not reduction can be (efficiently!) maintained too.
  if (marked_shortest_path_reduced())
    status.reset_shortest_path_reduced();

  // If we removed _all_ dimensions from a non-empty BDS,
  // the zero-dim universe BDS has been obtained.
  if (new_dim == 0 && !marked_empty())
    set_zero_dim_univ();
  assert(OK());
}

template <typename T>
inline bool
BD_Shape<T>::intersection_assign_and_minimize(const BD_Shape& y) {
  intersection_assign(y);
  shortest_path_closure_assign();
  return !marked_empty();
}

template <typename T>
inline void
BD_Shape<T>::CC76_extrapolation_assign(const BD_Shape& y, unsigned* tp) {
  static N stop_points[] = {
    N(-2, ROUND_UP),
    N(-1, ROUND_UP),
    N( 0, ROUND_UP),
    N( 1, ROUND_UP),
    N( 2, ROUND_UP)
  };
  CC76_extrapolation_assign(y,
			    stop_points,
			    stop_points
			    + sizeof(stop_points)/sizeof(stop_points[0]),
			    tp);
}

template <typename T>
inline void
BD_Shape<T>::H79_widening_assign(const BD_Shape& y, unsigned* tp) {
  // See the documentation for polyhedra.
  C_Polyhedron px(constraints());
  C_Polyhedron py(y.constraints());
  px.H79_widening_assign(py, tp);
  BD_Shape x(px);
  swap(x);
  assert(OK());
}

template <typename T>
inline void
BD_Shape<T>::limited_H79_extrapolation_assign(const BD_Shape& y,
					      const Constraint_System& cs,
					      unsigned* tp) {
  // See the documentation for polyhedra.
  C_Polyhedron px(constraints());
  C_Polyhedron py(y.constraints());
  px.limited_H79_extrapolation_assign(py, cs, tp);
  BD_Shape x(px);
  swap(x);
  assert(OK());
}

template <typename T>
inline void
BD_Shape<T>::time_elapse_assign(const BD_Shape& y) {
  // Dimension-compatibility check.
  if (space_dimension() != y.space_dimension())
    throw_dimension_incompatible("time_elapse_assign(y)", y);
  // See the documentation for polyhedra.
  C_Polyhedron px(constraints());
  C_Polyhedron py(y.constraints());
  px.time_elapse_assign(py);
  BD_Shape x(px);
  swap(x);
  assert(OK());
}

template <typename T>
inline void
BD_Shape<T>::forget_all_dbm_constraints(const dimension_type v) {
  assert(0 < v && v <= dbm.num_rows());
  DB_Row<N>& dbm_v = dbm[v];
  for (dimension_type i = dbm.num_rows(); i-- > 0; ) {
    dbm_v[i] = PLUS_INFINITY;
    dbm[i][v] = PLUS_INFINITY;
  }
}

template <typename T>
inline void
BD_Shape<T>::forget_binary_dbm_constraints(const dimension_type v) {
  assert(0 < v && v <= dbm.num_rows());
  DB_Row<N>& dbm_v = dbm[v];
  for (dimension_type i = dbm.num_rows()-1; i > 0; --i) {
    dbm_v[i] = PLUS_INFINITY;
    dbm[i][v] = PLUS_INFINITY;
  }
}

} // namespace Parma_Polyhedra_Library

namespace std {

/*! \relates Parma_Polyhedra_Library::BD_Shape */
template <typename T>
inline void
swap(Parma_Polyhedra_Library::BD_Shape<T>& x,
     Parma_Polyhedra_Library::BD_Shape<T>& y) {
  x.swap(y);
}

} // namespace std

#endif // !defined(PPL_BD_Shape_inlines_hh)

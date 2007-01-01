/* Floating point unit related functions.
   Copyright (C) 2001-2007 Roberto Bagnara <bagnara@cs.unipr.it>

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

#ifndef PPL_fpu_defs_hh
#define PPL_fpu_defs_hh 1

#include "fpu.types.hh"

namespace Parma_Polyhedra_Library {

//! Returns the current FPU rounding direction.
fpu_rounding_direction_type
fpu_get_rounding_direction();

//! Sets the FPU rounding direction to \p dir.
void
fpu_set_rounding_direction(fpu_rounding_direction_type dir);

/*! \brief
  Sets the FPU rounding direction to \p dir and returns the rounding
  control word previously in use.
*/
fpu_rounding_control_word_type
fpu_save_rounding_direction(fpu_rounding_direction_type dir);

/*! \brief
  Sets the FPU rounding direction to \p dir, clears the <EM>inexact
  computation</EM> status, and returns the rounding control word
  previously in use.
*/
fpu_rounding_control_word_type
fpu_save_rounding_direction_reset_inexact(fpu_rounding_direction_type dir);

//! Restores the FPU rounding rounding control word to \p cw.
void
fpu_restore_rounding_direction(fpu_rounding_control_word_type w);

//! Clears the <EM>inexact computation</EM> status.
void
fpu_reset_inexact();

/*! \brief
  Queries the <EM>inexact computation</EM> status.

  Returns 0 if the computation was definitely exact, 1 if it was
  definitely inexact, -1 if definite exactness information is unavailable.
*/
int
fpu_check_inexact();

} // namespace Parma_Polyhedra_Library

#if defined(__i386__)
#include "fpu-ia32.inlines.hh"
#elif defined(HAVE_FENV_H)
#include "fpu-c99.inlines.hh"
#elif defined(HAVE_IEEEFP_H)						\
  && (defined(__sparc)							\
      || defined(sparc)							\
      || defined(__sparc__))
#include "fpu-sparc.inlines.hh"
#else
#include "fpu-none.inlines.hh"
#endif

#endif // !defined(PPL_fpu_defs_hh)

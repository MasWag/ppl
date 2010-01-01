/* PPL_Object Java class declaration and implementation.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>

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

package parma_polyhedra_library;

/*!
  \cond Include_Implementation_Details
  \brief Smart pointer to a PPL, C++ object.
  \ingroup PPL_Java_interface
  \endcond
*/
public class PPL_Object {

    //! Stores the value of the C++ pointer.
    long ptr;

    //! Builds an object that points to `null'.
    protected PPL_Object() {
	ptr = 0;
    }

    private static native void initIDs();
    static {
        initIDs();
    }
}

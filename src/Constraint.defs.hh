/* Constraint class declaration.
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

#ifndef _Constraint_defs_hh
#define _Constraint_defs_hh 1

#include "Constraint.types.hh"
#include "Row.defs.hh"
#include "Variable.defs.hh"
#include "LinExpression.types.hh"
#include <iosfwd>

namespace Parma_Polyhedra_Library {

  //! Output operator.
  /*! \relates Constraint */
  std::ostream& operator <<(std::ostream& s, const Constraint& c);

  // Put them in the namespace here to declare them friend later.
  Constraint operator ==(const LinExpression& e1, const LinExpression& e2);
  Constraint operator ==(const LinExpression& e, const Integer& n);
  Constraint operator ==(const Integer& n, const LinExpression& e);

  Constraint operator <=(const LinExpression& e1, const LinExpression& e2);
  Constraint operator <=(const LinExpression& e, const Integer& n);
  Constraint operator <=(const Integer& n, const LinExpression& e);

  Constraint operator >=(const LinExpression& e1, const LinExpression& e2);
  Constraint operator >=(const LinExpression& e, const Integer& n);
  Constraint operator >=(const Integer& n, const LinExpression& e);

  Constraint operator >>(const Constraint& c, unsigned int offset);
}

//! A linear equality or inequality.
/*!
  An object of the class Constraint is either:
  - an equality: \f$\sum_{i=0}^{n-1} a_i x_i + b = 0\f$; or
  - an inequality: \f$\sum_{i=0}^{n-1} a_i x_i + b \geq 0\f$;

  where \f$n\f$ is the dimension of the space.

  \par How to build a constraint
  Constraints are typically built by applying a relational operator
  to a pair of linear expressions.
  Available relational operators include equality (<CODE>==</CODE>)
  and non-strict inequalities (<CODE>>=</CODE> and <CODE><=</CODE>).
  Strict inequalities (<CODE><</CODE> and <CODE>></CODE>)
  are not supported.
  The space-dimension of a constraint is defined as the maximum
  space-dimension of the arguments of its constructor.

  \par
  In the following examples it is assumed that variables
  <CODE>x</CODE>, <CODE>y</CODE> and <CODE>z</CODE>
  are defined as follows:
  \code
  Variable x(0);
  Variable y(1);
  Variable z(2);
  \endcode

  \par Example 1
  The following code builds the equality constraint
  \f$3x + 5y - z = 0\f$, having space-dimension \f$3\f$:
  \code
  Constraint eq_c(3*x + 5*y - z == 0);
  \endcode
  The following code builds the inequality constraint
  \f$4x \geq 2y - 13\f$, having space-dimension \f$2\f$:
  \code
  Constraint ineq_c(4*x >= 2*y - 13);
  \endcode
  The unsatisfiable constraint on the zero-dimension space \f$\Rset^0\f$
  can be specified as follows:
  \code
  Constraint false_c = Constraint::zero_dim_false();
  \endcode
  An equivalent, but more involved way is the following:
  \code
  Constraint false_c(LinExpression::zero() == 1);
  \endcode
  In constrast, the following code defines an unsatisfiable constraint
  having space-dimension \f$3\f$:
  \code
  Constraint false_c(0*z == 1);
  \endcode

  \par Example 2
  The following code prints all the coefficients of a given constraint:
  \code
  size_t c_space_dim = c.space_dimension();
  for (size_t varid = 0; varid < c_space_dim; varid++) {
    Variable v(varid);
    cout << "Variable " << v << " has coefficient "
         << c.coefficient(v) << endl;
  }
  cout << "The inhomogeneous term is "
       << c.coefficient() << endl;
  \endcode
  Namely, for a constraint defined by
  \code
  Constraint c(2*x <= 6*z - 7);
  \endcode
  the output is the following:
  \code
  Variable A has coefficient -2
  Variable B has coefficient 0
  Variable C has coefficient 6
  The inhomogeneous term is -7 
  \endcode
  Note the changes in the signs of the coefficients: when constraints
  are actually built, all <CODE><=</CODE> inequalities are transformed
  into <CODE>>=</CODE> and then all the terms are moved to
  the left-hand side of the inequality.
  When dealing with equalities, the actual sign of the
  coefficients is somehow unpredictable, since they could have been
  (consistently!) negated.
  For instance, if the initial constraint was defined by
  \code
  Constraint c(5 == -3*y + 6*z);
  \endcode
  then we would have obtained the following output:
  \code
  Variable A has coefficient 0
  Variable B has coefficient 3
  Variable C has coefficient -6
  The inhomogeneous term is 5
  \endcode
  Finally, by using the following code, it is possible to
  automatically skip all the variable coefficients that are
  equal to zero:
  \code
  for (int varid = c.first(); varid >= 0; varid = c.next(varid)) {
    Variable v(varid);
    cout << "Variable " << v << " has coefficient "
         << c.coefficient(v) << endl;
  }
  cout << "The inhomogeneous term is "
       << c.coefficient() << endl;
  \endcode
  Therefore, for the last constraint defined above,
  the output would have been:
  \code
  Variable B has coefficient 3
  Variable C has coefficient -6
  The inhomogeneous term is 5
  \endcode
*/
class Parma_Polyhedra_Library::Constraint : PPL_INTERNAL Row {
private:
  //! Default constructor: private and not implemented.
  Constraint();

  Constraint(LinExpression& e);

  Constraint(Row::Type type, size_t size);

  //! Returns the constraint \p e1 = \p e2.
  friend Constraint
  Parma_Polyhedra_Library::operator ==(const LinExpression& e1,
				       const LinExpression& e2);

  //! Returns the constraint \p e = \p n.
  friend Constraint
  Parma_Polyhedra_Library::operator ==(const LinExpression& e,
				       const Integer& n);

  //! Returns the constraint \p n = \p e.
  friend Constraint
  Parma_Polyhedra_Library::operator ==(const Integer& n,
				       const LinExpression& e);

  //! Returns the constraint \p e1 >= \p e2.
  friend Constraint
  Parma_Polyhedra_Library::operator >=(const LinExpression& e1,
				       const LinExpression& e2);

  //! Returns the constraint \p e >= \p n.
  friend Constraint
  Parma_Polyhedra_Library::operator >=(const LinExpression& e,
				       const Integer& n);

  //! Returns the constraint \p n >= \p e.
  friend Constraint
  Parma_Polyhedra_Library::operator >=(const Integer& n,
				       const LinExpression& e);

  //! Returns the constraint \p e1 <= \p e2.
  friend Constraint
  Parma_Polyhedra_Library::operator <=(const LinExpression& e1,
				       const LinExpression& e2);

  //! Returns the constraint \p e <= \p n.
  friend Constraint
  Parma_Polyhedra_Library::operator <=(const LinExpression& e,
				       const Integer& n);

  //! Returns the constraint \p n <= \p e.
  friend Constraint
  Parma_Polyhedra_Library::operator <=(const Integer& n,
				       const LinExpression& e);

  //! Returns the constraint \p c with variables renamed
  //! by adding \p offset to their Cartesian axis identifier.
  friend Constraint
  Parma_Polyhedra_Library::operator >>(const Constraint& c,
				       unsigned int offset);

public:
  //! Ordinary copy-constructor.
  Constraint(const Constraint& c);
  //! Destructor.
  ~Constraint();

  //! Returns the dimension of the vector space enclosing \p *this.
  size_t space_dimension() const;

  //! Returns <CODE>true</CODE> if and only if
  //! \p *this is an equality constraint.
  bool is_equality() const;
  //! Returns <CODE>true</CODE> if and only if
  //! \p *this is an inequality constraint.
  bool is_inequality() const;

  //! If the index of variable \p v is less than the space-dimension
  //! of \p *this, returns the coefficient of \p v in \p *this.
  //! \exception std::invalid_argument thrown if the index of \p v
  //! is greater than or equal to the space-dimension of \p *this.
  const Integer& coefficient(Variable v) const;
  //! Returns the inhomogeneous term of \p *this.
  const Integer& coefficient() const;

  //! The unsatisfiable (zero-dimension space) constraint \f$0 = 1\f$.
  static const Constraint& zero_dim_false();

  //! The true (zero-dimension space) constraint \f$0 \leq 1\f$,
  //! also known as <EM>positivity constraint</EM>.
  static const Constraint& zero_dim_positivity();

PPL_INTERNAL:
  //! Returns <CODE>true</CODE> if and only if
  //! \p *this is the trivial true constraint \f$0 <= n\f$,
  //! where \f$n \geq 0\f$.
  bool is_trivial_true() const;

  //! Returns <CODE>true</CODE> if and only if
  //! \p *this is the trifial false constraint
  //! (i.e., either \f$0 >= n\f$, where \f$n > 0\f$
  //! or \f$0 = n\f$, where \f$n \neq 0\f$.
  bool is_trivial_false() const;

  enum Type {
    EQUALITY = Row::LINE_OR_EQUALITY,
    INEQUALITY = Row::RAY_OR_VERTEX_OR_INEQUALITY
  };

  //! Returns the constraint type of \p *this.
  Type type() const;
  //! Sets the constraint type to <CODE>EQUALITY</CODE>.
  void set_is_equality();
  //! Sets the constraint type to <CODE>INEQUALITY</CODE>.
  void set_is_inequality();
};

namespace std {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Specialize <CODE>std::swap</CODE>.
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
void swap(Parma_Polyhedra_Library::Constraint& x,
	  Parma_Polyhedra_Library::Constraint& y);

} // namespace std


#include "Constraint.inlines.hh"

#endif

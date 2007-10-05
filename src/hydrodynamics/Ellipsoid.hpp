/*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Acknowledgement of the program authors must be made in any
 *    publication of scientific results based in part on use of the
 *    program.  An acceptable form of acknowledgement is citation of
 *    the article in which the program was described (Matthew
 *    A. Meineke, Charles F. Vardeman II, Teng Lin, Christopher
 *    J. Fennell and J. Daniel Gezelter, "OOPSE: An Object-Oriented
 *    Parallel Simulation Engine for Molecular Dynamics,"
 *    J. Comput. Chem. 26, pp. 252-271 (2005))
 *
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. All express or implied conditions, representations and
 * warranties, including any implied warranty of merchantability,
 * fitness for a particular purpose or non-infringement, are hereby
 * excluded.  The University of Notre Dame and its licensors shall not
 * be liable for any damages suffered by licensee as a result of
 * using, modifying or distributing the software or its
 * derivatives. In no event will the University of Notre Dame or its
 * licensors be liable for any lost revenue, profit or data, or for
 * direct, indirect, special, consequential, incidental or punitive
 * damages, however caused and regardless of the theory of liability,
 * arising out of the use of or inability to use software, even if the
 * University of Notre Dame has been advised of the possibility of
 * such damages.
 */

#ifndef HYDRODYNAMICS_ELLIPSOID_HPP
#define HYDRODYNAMICS_ELLIPSOID_HPP
#include "hydrodynamics/Shape.hpp"
#include "math/SquareMatrix3.hpp"
namespace oopse {
  
  /** @class Ellipsoid 
   *
   * An ellipsoid in OOPSE is restricted to having two equal
   * equatorial semi-axes.  OOPSE treats the "special" axis as the
   * z-coordinate in the body fixed reference frame.  That is:
   *
   *   z^2 / a^2  +  (x^2 + y^2) / b^2   = 1
   *
   * If a >= b, the ellipsoid is a prolate spheroid, and if a < b, 
   * the ellipsoid is oblate.  Ellipsoids are specified in the constructor 
   * using an axial length (a), and a equatorial length (b).  A Vector3d 
   * can be used to position the center of the ellipsoid, and a rotation 
   * matrix can also be used to orient the ellipsoid to a preferred lab-fixed
   * coordinate frame.
   */
  class Ellipsoid : public Shape{
  public:
    Ellipsoid(Vector3d origin, RealType rAxial, RealType rEquatorial, Mat3x3d rotMat);
    virtual bool isInterior(Vector3d pos);
    virtual std::pair<Vector3d, Vector3d> getBoundingBox();
    virtual bool hasAnalyticalSolution() {return true;}
    
    virtual HydroProp* getHydroProp(RealType viscosity, RealType temperature);
    
    RealType getRAxial() {return rAxial_;}
    RealType getREquatorial() {return rEquatorial_;}
    RealType getRMajor() {return rMajor_;}
    RealType getRMinor() {return rMinor_;}
    
  private:
    
    Vector3d origin_;
    RealType rAxial_;
    RealType rEquatorial_;
    RealType rMajor_;
    RealType rMinor_;
    Mat3x3d rotMat_;
  };  
}

#endif

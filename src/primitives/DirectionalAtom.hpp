/*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
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
 *
 * SUPPORT OPEN SCIENCE!  If you use OpenMD or its source code in your
 * research, please cite the appropriate papers when you publish your
 * work.  Good starting points are:
 *                                                                      
 * [1]  Meineke, et al., J. Comp. Chem. 26, 252-271 (2005).             
 * [2]  Fennell & Gezelter, J. Chem. Phys. 124, 234104 (2006).          
 * [3]  Sun, Lin & Gezelter, J. Chem. Phys. 128, 234107 (2008).          
 * [4]  Kuang & Gezelter,  J. Chem. Phys. 133, 164101 (2010).
 * [5]  Vardeman, Stocker & Gezelter, J. Chem. Theory Comput. 7, 834 (2011).
 */
 
/**
 * @file DirectionalAtom.hpp
 * @author    tlin
 * @date  10/23/2004
 * @version 1.0
 */ 

#ifndef PRIMITIVES_DIRECTIONALATOM_HPP
#define PRIMITIVES_DIRECTIONALATOM_HPP

#include "primitives/Atom.hpp"
#include "types/AtomType.hpp"
namespace OpenMD{
  class DirectionalAtom : public Atom {
  public:
    DirectionalAtom(AtomType* dAtomType);
    /**
     * Returns the inertia tensor of this stuntdouble
     * @return the inertia tensor of this stuntdouble
     */ 
    virtual Mat3x3d getI();            
    
    /**
     * Sets  the previous rotation matrix of this stuntdouble
     * @param a  new rotation matrix 
     */         
    virtual void setPrevA(const RotMat3x3d& a);
    
    /**
     * Sets  the current rotation matrix of this stuntdouble
     * @param a  new rotation matrix 
     */         
    virtual void setA(const RotMat3x3d& a);
    
    /**
     * Sets  the rotation matrix of this stuntdouble in specified snapshot
     * @param a rotation matrix to be set 
     * @param snapshotNo 
     * @see #getA
     */         
    virtual void setA(const RotMat3x3d& a, int snapshotNo);
    
    /** 
     * Left multiple rotation matrix by another rotation matrix 
     * @param m a rotation matrix
     */
    void rotateBy(const RotMat3x3d& m);
        
    /**
     * Returns the gradient of this stuntdouble
     * @return the gradient of this stuntdouble
     */ 
    virtual std::vector<RealType> getGrad();
    
    virtual void accept(BaseVisitor* v);
   
  private:
    Mat3x3d I_;
    Vector3d dipole_;    // body fixed dipole vector
    Mat3x3d quadrupole_; // body fixed quadrupole tensor
  };  
}//namespace OpenMD
#endif //PRIMITIVES_DIRECTIONALATOM_HPP

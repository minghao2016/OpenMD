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
  
#include "integrators/Velocitizer.hpp"
#include "math/SquareMatrix3.hpp"
#include "primitives/Molecule.hpp"
#include "primitives/StuntDouble.hpp"
#include "math/randomSPRNG.hpp"

namespace oopse {

void Velocitizer::velocitize(double temperature) {
    Vector3d aVel;
    Vector3d aJ;
    Mat3x3d I;
    int l;
    int m;
    int n; 
    Vector3d vdrift;
    double vbar;
    /**@todo refactory kb */
    const double kb = 8.31451e-7; // kb in amu, angstroms, fs, etc.
    double av2;
    double kebar;

    SimInfo::MoleculeIterator i;
    Molecule::IntegrableObjectIterator j;
    Molecule * mol;
    StuntDouble * integrableObject;
    gaussianSPRNG gaussStream(info_->getSeed());

    kebar = kb * temperature * info_->getNdfRaw() / (2.0 * info_->getNdf());

    for( mol = info_->beginMolecule(i); mol != NULL;
        mol = info_->nextMolecule(i) ) {
        for( integrableObject = mol->beginIntegrableObject(j);
            integrableObject != NULL;
            integrableObject = mol->nextIntegrableObject(j) ) {

            // uses equipartition theory to solve for vbar in angstrom/fs

            av2 = 2.0 * kebar / integrableObject->getMass();
            vbar = sqrt(av2);

            // picks random velocities from a gaussian distribution
            // centered on vbar

            for( int k = 0; k < 3; k++ ) {
                aVel[k] = vbar * gaussStream.getGaussian();
            }

            integrableObject->setVel(aVel);

            if (integrableObject->isDirectional()) {
                I = integrableObject->getI();

                if (integrableObject->isLinear()) {
                    l = integrableObject->linearAxis();
                    m = (l + 1) % 3;
                    n = (l + 2) % 3;

                    aJ[l] = 0.0;
                    vbar = sqrt(2.0 * kebar * I(m, m));
                    aJ[m] = vbar * gaussStream.getGaussian();
                    vbar = sqrt(2.0 * kebar * I(n, n));
                    aJ[n] = vbar * gaussStream.getGaussian();
                } else {
                    for( int k = 0; k < 3; k++ ) {
                        vbar = sqrt(2.0 * kebar * I(k, k));
                        aJ[k] = vbar * gaussStream.getGaussian();
                    }
                } // else isLinear

                integrableObject->setJ(aJ);
            }     //isDirectional 
        }
    }             //end for (mol = beginMolecule(i); ...)



    removeComDrift();

}



void Velocitizer::removeComDrift() {
    // Get the Center of Mass drift velocity.
    Vector3d vdrift = info_->getComVel();
    
    SimInfo::MoleculeIterator i;
    Molecule::IntegrableObjectIterator j;
    Molecule * mol;
    StuntDouble * integrableObject;
    
    //  Corrects for the center of mass drift.
    // sums all the momentum and divides by total mass.
    for( mol = info_->beginMolecule(i); mol != NULL;
        mol = info_->nextMolecule(i) ) {
        for( integrableObject = mol->beginIntegrableObject(j);
            integrableObject != NULL;
            integrableObject = mol->nextIntegrableObject(j) ) {
            integrableObject->setVel(integrableObject->getVel() - vdrift);
        }
    }

}

}

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

#include <algorithm>

#include "RadialDistrFunc.hpp"
#include "io/DumpReader.hpp"
#include "primitives/Molecule.hpp"
namespace oopse {

RadialDistrFunc::        RadialDistrFunc(SimInfo* info, const std::string& filename, const std::string& sele1, const std::string& sele2)
        : info_(info), currentSnapshot_(NULL), dumpFilename_(filename), step_(1), 
          selectionScript1_(sele1), selectionScript2_(sele2), evaluator1_(info), evaluator2_(info), seleMan1_(info), seleMan2_(info){
          
    evaluator1_.loadScriptString(sele1);
    evaluator2_.loadScriptString(sele2);

    if (!evaluator1_.isDynamic()) {
            seleMan1_.setSelectionSet(evaluator1_.evaluate());
    }
    if (!evaluator2_.isDynamic()) {
            seleMan2_.setSelectionSet(evaluator2_.evaluate());
    }

    if (!evaluator1_.isDynamic() && !evaluator2_.isDynamic()) {
        //if all selections are static,  we can precompute the number of real pairs    

        int nSelected1 = seleMan1_.getSelectionCount();
        int nSelected2 = seleMan2_.getSelectionCount();

        BitSet bs = seleMan1_.getSelectionSet();
        bs &= seleMan2_.getSelectionSet();
        int nIntersect = bs.countBits();

        nRealPairs_ = nSelected1 * nSelected2 - (nIntersect +1) * nIntersect/2;
    }
    
}

void RadialDistrFunc::process() {
    Molecule* mol;
    RigidBody* rb;
    SimInfo::MoleculeIterator mi;
    Molecule::RigidBodyIterator rbIter;
    
    preProcess();
    
    DumpReader reader(info_, dumpFilename_);    
    int nFrames = reader.getNFrames();
    nProcessed_ = nFrames / step_;

    for (int i = 0; i < nFrames; i += step_) {
        reader.readFrame(i);
        currentSnapshot_ = info_->getSnapshotManager()->getCurrentSnapshot();

        if (evaluator1_.isDynamic()) {
            seleMan1_.setSelectionSet(evaluator1_.evaluate());
        }
        if (evaluator2_.isDynamic()) {
            seleMan2_.setSelectionSet(evaluator2_.evaluate());
        }

        for (mol = info_->beginMolecule(mi); mol != NULL; mol = info_->nextMolecule(mi)) {

            //change the positions of atoms which belong to the rigidbodies
            for (rb = mol->beginRigidBody(rbIter); rb != NULL; rb = mol->nextRigidBody(rbIter)) {
                rb->updateAtoms();
            }
        }
        
        initalizeHistogram();

        StuntDouble* sd1;
        int j;
        for (sd1 = seleMan1_.beginSelected(j); sd1 != NULL; sd1 = seleMan1_.nextSelected(j)) {

            StuntDouble* sd2;
            int k;
            for (sd2 = seleMan2_.beginSelected(k); sd2 != NULL; sd2 = seleMan2_.nextSelected(k)) {
                if (sd1 != sd2) {
                    collectHistogram(sd1, sd2);
                }
            }            
        }

        processHistogram();
        
    }

    postProcess();

    writeRdf();
}

int RadialDistrFunc::getNRealPairs() {
    if (evaluator1_.isDynamic() || evaluator2_.isDynamic()) {
        //if one of the selection is dynamic,  need to recompute it    

        int nSelected1 = seleMan1_.getSelectionCount();
        int nSelected2 = seleMan2_.getSelectionCount();

        BitSet bs = seleMan1_.getSelectionSet();
        bs &= seleMan2_.getSelectionSet();
        int nIntersect = bs.countBits();

        nRealPairs_ = nSelected1 * nSelected2 - (nIntersect +1) * nIntersect/2;
    }

    return nRealPairs_;
}

}
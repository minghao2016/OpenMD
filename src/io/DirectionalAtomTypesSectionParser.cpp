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
 
#include "io/DirectionalAtomTypesSectionParser.hpp"
#include "UseTheForce/ForceField.hpp"
#include "types/DirectionalAtomType.hpp"

namespace oopse {

DirectionalAtomTypesSectionParser::DirectionalAtomTypesSectionParser() {
    setSectionName("DirectionalAtomTypes");
}

void DirectionalAtomTypesSectionParser::parseLine(ForceField& ff,const std::string& line, int lineNo){
    StringTokenizer tokenizer(line);
    int nTokens = tokenizer.countTokens();    

    //in AtomTypeSection, a line at least contains 2 tokens
    //atomTypeName and mass
    if (nTokens < 4)  {
        std::cerr << "DirectionalAtomTypesSectionParser:: Not enought Tokens" << std::endl;                       
    } else {

        std::string atomTypeName = tokenizer.nextToken();    
        AtomType* atomType = ff.getAtomType(atomTypeName);
        DirectionalAtomType* dAtomType;
        if (atomType == NULL) {
            dAtomType = new DirectionalAtomType();
            int ident = ff.getNAtomType() + 1;
            dAtomType->setIdent(ident); 
            dAtomType->setName(atomTypeName);
            ff.addAtomType(atomTypeName, dAtomType);
        } else {
            dAtomType = dynamic_cast<DirectionalAtomType*>(atomType);
            if (dAtomType == NULL) {
                std::cerr << "DirectionalAtomTypesSectionParser:: Can not cast to DirectionalAtomType" << std::endl;                       
            }
        }

        double Ixx = tokenizer.nextTokenAsDouble();
        double Iyy = tokenizer.nextTokenAsDouble();
        double Izz = tokenizer.nextTokenAsDouble();            
        Mat3x3d inertialMat;
        inertialMat(0, 0) = Ixx;
        inertialMat(1, 1) = Iyy;
        inertialMat(2, 2) = Izz;        
        dAtomType->setI(inertialMat);            
                       
    }    

}

} //end namespace oopse
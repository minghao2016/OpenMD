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
 
#ifndef VISITORS_OTHERVISITOR_HPP
#define VISITORS_OTHERVISITOR_HPP
#include <set>
#include <string>
#include <vector>

#include "visitors/BaseVisitor.hpp"
#include "primitives/StuntDouble.hpp"
#include "visitors/AtomData.hpp"


namespace oopse {

class SimInfo;

//IgnoreVisitor will turn on the ignoring flag of the stuntdouble
class IgnoreVisitor : public BaseVisitor{
  public:
    IgnoreVisitor() : BaseVisitor() {visitorName = "IgnoreVisitor";}

    virtual void visit(Atom* atom);
    virtual void visit(DirectionalAtom* datom);
    virtual void visit(RigidBody* rb);
    
    virtual const std::string toString();

    void addIgnoreType(const std::string& type) {itList.insert(type);}
    
  protected:
    bool isIgnoreType(const std::string& name);
    void internalVisit(StuntDouble* sd);
    std::set<std::string> itList; //ignore type list;
};


class WrappingVisitor : public BaseVisitor{
  public:
    WrappingVisitor(SimInfo* info) : BaseVisitor() {
      this->info = info;
      visitorName = "WrappingVisitor";
    }
    virtual void visit(Atom* atom);
    virtual void visit(DirectionalAtom* datom);
    virtual void visit(RigidBody* rb);

    virtual const std::string toString();

  protected:
    void internalVisit(StuntDouble* sd);
    SimInfo* info;
};


class ReplicateVisitor : public BaseVisitor{
  public:
    ReplicateVisitor(SimInfo* info, Vector3i opt);
    virtual void visit(Atom* atom);
    virtual void visit(DirectionalAtom* datom);
    virtual void visit(RigidBody* rb);

    virtual const std::string toString();
  protected:
    void internalVisit(StuntDouble* sd);
    void replicate(std::vector<AtomInfo*>& infoList,  AtomData* data, const Mat3x3d& box);
    
  private:
    std::vector<Vector3i> dir;
    SimInfo* info;
    Vector3i replicateOpt;
};

class XYZVisitor : public BaseVisitor{
  public:
    XYZVisitor(SimInfo* info, bool printDipole = true);
    
    virtual void visit(Atom* atom);
    virtual void visit(DirectionalAtom* datom);
    virtual void visit(RigidBody* rb);

    virtual const std::string toString();
    
    void writeFrame(std::ostream& outStream);    
    void clear() {frame.clear();}
    
  protected:
    void internalVisit(StuntDouble* sd);
    bool isIgnore(StuntDouble* sd);

  private:  
    SimInfo* info;
    std::vector<std::string> frame;
    bool printDipole;
};


class PrepareVisitor : public BaseVisitor{
  public:
    PrepareVisitor() : BaseVisitor() {visitorName = "prepareVisitor";}

    virtual void visit(Atom* atom) {internalVisit(atom);}
    virtual void visit(DirectionalAtom* datom) {internalVisit((Atom*)datom);}
    virtual void visit(RigidBody* rb) {internalVisit(rb);}

    virtual const std::string toString();

  protected:
    void internalVisit(Atom* atom);
    void internalVisit(RigidBody* rb);
};

class WaterTypeVisitor : public BaseVisitor{
  public:
    WaterTypeVisitor() ;
    virtual void visit(Atom* atom) {}
    virtual void visit(DirectionalAtom* datom) {}
    virtual void visit(RigidBody* rb);

    virtual const std::string toString();
    
  private:
    void replaceType(std::string& atomType);
      
    std::set<std::string> waterTypeList;
};


}//namespace oopse
#endif //_OTHERVISITOR_H_

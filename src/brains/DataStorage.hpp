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
 
/**
 * @file Vector.hpp
 * @author Teng Lin
 * @date 09/14/2004
 * @version 1.0
 */

#ifndef BRAINS_DATASTORAGE_HPP
#define BRAINS_DATASTORAGE_HPP

#include <vector>
#include <math/Vector3.hpp>
#include <math/SquareMatrix3.hpp>

using namespace oopse;

    /**
     * @class DataStorage
     * @warning do not try to insert element into (or ease element from) private member data 
     * of DataStorage directly.
     * @todo DataStorage may need refactorying. Every std::vector can inherit from the same base class
     * which will make it easy to maintain
     */
    class DataStorage {
        public:

            enum{
                dslPosition = 1,
                dslVelocity = 2,
                dslAmat = 4, 
                dslAngularMomentum = 8,
                dslElectroFrame = 16,
                dslZAngle = 32,
                dslForce = 64, 
                dslTorque = 128
            };


            DataStorage();
            DataStorage(int size, int storageLayout = 255);
            /** return the size of this DataStorage. */
            int getSize();
            /**
             * Changes the size of this DataStorage.
             * @param size new size of this DataStorage
             */
            void resize(int newSize);
            /**
             * Reallocates memory manually. The main reason for using reserve() is efficiency
             * if you know the capacity to which your std::vector must eventually grow, then it is usually more
             * efficient to allocate that memory all at once.
             */
            void reserve(int size);
            /**
             * Copies data inside DataStorage class.
             * Copy function actually call std::copy for every std::vector in DataStorage class. 
             * One Precondition of std::copy is that target is not within the range [soruce, soruce + num]
             * @param souce 
             * @param num number of element to be moved
             * @param target
             */
            void copy(int source, int num, int target);
            /** Returns the storage layout  */
            int getStorageLayout();
            /** Sets the storage layout  */
            void setStorageLayout(int layout);
            /** Returns the pointer of internal array */
            double *getArrayPointer(int whichArray);

            std::vector<Vector3d> position;               /** position array */
            std::vector<Vector3d> velocity;               /** velocity array */
            std::vector<RotMat3x3d> aMat;            /** rotation matrix array */
            std::vector<Vector3d> angularMomentum;/** angular momentum array (body-fixed) */
            std::vector<Mat3x3d> electroFrame;                /** the lab frame unit std::vector array*/
            std::vector<double> zAngle;              /** z -angle array */        
            std::vector<Vector3d> force;               /** force array */
            std::vector<Vector3d> torque;               /** torque array */

        private:

            double* internalGetArrayPointer(std::vector<Vector3d>& v);
            
            double* internalGetArrayPointer(std::vector<RotMat3x3d>& v);
            double* internalGetArrayPointer(std::vector<double>& v);
            
            template<typename T>
            void internalResize(std::vector<T>& v, int newSize);

            template<typename T>
            void internalCopy(std::vector<T>& v, int source,  int num, int target);
            
            int size_;
            int storageLayout_;

    };


#endif //BRAINS_DATASTORAGE_HPP

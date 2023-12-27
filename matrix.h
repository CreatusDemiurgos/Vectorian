#ifndef MATRIX_H
#define MATRIX_H
#include <cstdlib>


//even if not specified, I am using this matrix class supposing that every matrix that I encounter is going to be a square one, this makes 
// calculations and some functions easier to implement and, for now, I don't really need a non-square matrices so there is that.
class Matrix{
    public:
        long double* matrix;
        int rowCount; //(for square matrices, also column count)
        void creatingMatrix(int nVars);
        void addElementToMatrix(float el, int pos);
        long double* minorMatrix(int posVar);
        long double calc4x4Determinant();
        long double calc3x3Determinant(long double* matrixToUse);
        long double* cofactorMatrix();
        long double* transposeMatrix(long double* matrixToUse);
        long double* adjoint();
        long double* inverseOfMatrix();
        void deletingMatrix();

};

#endif 
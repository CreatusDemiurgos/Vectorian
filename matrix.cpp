#include "matrix.h"
#include <iostream>
#include <cmath>


//nVars stands for how many variables are in ONE row, not the entire matrix
void Matrix::creatingMatrix(int nVars){
    rowCount = nVars;
    matrix = new long double[nVars * nVars]; 
}
void Matrix::deletingMatrix(){
    delete matrix;
    //std::cout << "\nb";
}

void Matrix::addElementToMatrix(float el, int pos){
    //std::cout << "\nc";
    matrix[pos-1] = el;
    //std::cout << "\n b" << el << '\n' << pos;
}

long double* Matrix::minorMatrix(int posVar){
   // std::cout << "\nd";
    posVar--; //giving the position of the position without indicating the row and column, e.g: 5 = row 1, column 2. This is done for every function in the class
    int posElimanteData;
    int j = 1;
    int i = 1;
    int *eliminatePos = new int[(2 * rowCount) - 2];// the minus 2 is used since: an array start from 0, and the row count starts from 1,
    // so we need to remove 1, the column and row that we need to eliminate have a number in common, and we need to count is only 1 time, so another minus 1
    //The "eliminatePos" array doesnt contain the values that we want to eliminate to create the minor matrix but it contains the positions of these values
    while(j > 0){
        //getting the values of the members of the column containing the value we want to delete that are less than the value itself
        j = posVar - (rowCount * i);
        if(j >= 0){
            eliminatePos[i - 1] = j;
            i++;
        }  
    }
    int k = 1;
    while(j < (rowCount * rowCount)){
        //getting the values of the members of the column containing the value we want to delete that are greater than the value itself
        j = posVar + (rowCount * k);
        if(j < (rowCount * rowCount)){
            eliminatePos[i + k - 2] = j;
            k++;
        } 
    }
    for(int l = static_cast<int>(posVar / rowCount)*rowCount; l <static_cast<int>(posVar / rowCount)*rowCount + rowCount; l ++ ){
        //initialization:getting the first variables of the row containing the value we want to eliminate, stops when it reaches the end of said column
        eliminatePos[i + k - 2] = l;
        i++;
    }
    int o = 0;//needed since it indicates the positions of the minor matrix

    /*for(int i  = 0; i<7; i++){
        std::cout << "numbers to be removed:" << *(eliminatePos + i) << '\n';
    }*/
    
    long double* minorMatrix = new long double[(rowCount -1) * (rowCount -1)];
    for(int m = 0; m <(rowCount) * (rowCount); m++){
        // for loop with original matrix length
        int p = 0;
        for(int n = 0; n <(2 * rowCount) - 1; n++){//for loop for the array with the values we want to eliminate
            if(m != (eliminatePos[n])){//checking if m, the position of the og matrix, isnt a position that we want to delete
                p++;
                if( p == (2 * rowCount) - 1){//p becomes this value if m is different from all of the values we want to delete, as a consequence we can get it
                    minorMatrix[o] = matrix[m];
                    o++;
                    p = 0;
                }
                
            }
        }
    }
    /*for(int i = 0; i < 9; i++){
        std::cout << *(minorMatrix + i) << "aa" << i << "\n";
    }*/
    //delete eliminatePos;
    return minorMatrix;
}


long double Matrix::calc4x4Determinant(){
    //std::cout << "\ne";
    float determinant;
    long double* minorMat1 = Matrix::minorMatrix(1);
    long double* minorMat2 = Matrix::minorMatrix(2);
    long double* minorMat3 = Matrix::minorMatrix(3);
    long double* minorMat4 = Matrix::minorMatrix(4);

    determinant = matrix[0] * this->calc3x3Determinant(minorMat1) - matrix[1] * this->calc3x3Determinant(minorMat2)
    + matrix[2] * this->calc3x3Determinant(minorMat3) - matrix[3] * this->calc3x3Determinant(minorMat4);

    //delete minorMat1;
    //delete minorMat2;
    //delete minorMat3;
    //delete minorMat4;

    //std::cout << '\n' << determinant << " uwu";

    return determinant;
    
}

long double Matrix::calc3x3Determinant(long double* matrixToUse){
    //std::cout << "\nf";
    long double determinant = 0;

    determinant = (matrixToUse[0]*matrixToUse[4]*matrixToUse[8]) + (matrixToUse[1]*matrixToUse[5]*matrixToUse[6]) + (matrixToUse[2]*matrixToUse[3]*matrixToUse[7])
    - (matrixToUse[2]*matrixToUse[4]*matrixToUse[6]) - (matrixToUse[1]*matrixToUse[3]*matrixToUse[8]) - (matrixToUse[0]*matrixToUse[5]*matrixToUse[7]);
    //std::cout << '\n' << determinant << " uwu3x3";

    return determinant;
}

long double* Matrix::cofactorMatrix(){
    //std::cout << "\ni";
    // the following code only works if the matrix is even (2x2, 4x4, 6x6...)
    long double* cMat = new long double[rowCount * rowCount];
    int k = 0;
    for(int i = 0; i < (rowCount * rowCount); i++){
        if((i % 4) == 0 && i != 0){
            k++;//this is needed to know in which row we are
        }
        long double* minorMat = this->minorMatrix(i + 1);
        cMat[i] = pow(-1, (i+k)) * this->calc3x3Determinant(minorMat);
        //delete minorMat;

       // std:: cout << '\n' << cMat[i];
    }

    return cMat;
}


long double* Matrix::transposeMatrix(long double* matrixToUse){
    //std::cout << "\nj";
    long double *transposeMatrix = new long double[rowCount * rowCount];
    int k = 4;
    int l = 0;
    transposeMatrix[0] = matrixToUse[0];
    for(int i = 1; i< 16; i++){
        transposeMatrix[i] = matrixToUse[k + l];
        k+= 4;
        if(k >= 16){
            l++;
            k = 0;
        }
    }
    /*for(int i = 0; i < 16; i++){
        std:: cout << '\n' << matrixToUse[i];
    }
    for(int i = 0; i < 16; i++){
        std:: cout << "\n a" << transposeMatrix[i];
    }*/

    return transposeMatrix;
}


long double* Matrix::adjoint(){
   // std::cout << "\nk";
    long double* cof = this->cofactorMatrix();
    long double* adj = this->transposeMatrix(cof);
    /*for(int i = 0; i < (rowCount * rowCount); i++){
        std::cout << "\n ILY" << adj[i];
    }*/
    //delete cof;
    return adj;
}

long double* Matrix::inverseOfMatrix(){
    //std::cout << "\nl";
    long double* adj = this->adjoint();
    long double* invMat = new long double[rowCount * rowCount];
    for(int i = 0; i < (rowCount * rowCount); i++){
        invMat[i] = (1/this->calc4x4Determinant()) * adj[i];
        //std::cout << ",\n " << invMat[i] << " a " << this->calc4x4Determinant() << " b " << adj[i];
    }
    //delete adj;
    return invMat;
}
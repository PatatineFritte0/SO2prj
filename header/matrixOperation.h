#ifndef MATRIXOPERATION_H
#define MATRIXOPERATION_H
#include "structure.h"

Complex** matrixMoltiplication(Complex**, Complex**, int);
Complex* mulMatrixByVector(Complex**, const Complex*, int);
Complex** createMatrix2D(int, int);
void freeMatrix2D(Complex**, int);
void freeMatrix3D(Complex***, int, int);
Complex*** createMatrix3D(int, int, int);

#endif //MATRIXOPERATION_H

#ifndef MATRIXOPERATION_H
#define MATRIXOPERATION_H

#include "structure.h"

/* Funzione per moltiplicare matrici in thread */
void *mulMatrixThreadFunction(void *);

/* Moltiplica due matrici quadrate di dimensione dim */
Complex** matrixMoltiplication(Complex**, Complex**, int);

/* Moltiplica una matrice per un vettore complesso */
Complex* mulMatrixByVector(Complex**, const Complex*, int);

/* Alloca una matrice 2D di dimensioni x*y */
Complex** createMatrix2D(int x, int y);

/* Libera una matrice 2D con dimensione dim */
void freeMatrix2D(Complex**, int dim);

/* Libera una matrice 3D di dimensioni x*y*z */
void freeMatrix3D(Complex***, int x, int y);

/* Alloca una matrice 3D di dimensioni x*y*z */
Complex*** createMatrix3D(int x, int y, int z);

/* Copia la matrice m2 in m1, con dimensione row*col */
void copyMatrix(Complex**, Complex**, int row, int col);

#endif /* MATRIXOPERATION_H */


#ifndef FUNCTION_H
#define FUNCTION_H

#include "costants.h"
#include "structure.h"

char* readFile(const char*);

int getNqbit (char*);
bool getInitVector(char*, Complex*, int);
Complex*** getMatrix(int, const char*, int , char[MAX_MATRIX][MAX_NAME_MATRIX]);
bool getOrderMatrix(const char*, char[MAX_MATRIX][MAX_NAME_MATRIX]);
Complex stringToComplex(const char* );

Complex** matrixMoltiplication(Complex**, Complex**, int);
Complex* mulMatrixByVector(Complex**, const Complex*, int);
Complex** createMatrix2D(int, int);
void freeMatrix2D(Complex**, int);
void freeMatrix3D(Complex***, int, int);
Complex*** createMatrix3D(int, int, int);

bool isPresent(const char*, const char*, int*);
bool isVectorCorrect(const Complex*, int);
char *deleteChar(const char*, char);
int count_char(const char*, char);
void printComplex(Complex);
void printVector(const Complex*, int);
void printMatrix(Complex**, int);
void insert_char(char* , char, int);

#endif //FUNCTION_H

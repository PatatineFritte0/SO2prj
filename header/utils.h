#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

#include "structure.h"
#include "boolCompact.h"

bool isPresent(const char*, const char*, int*);
bool isVectorCorrect(const Complex*, int);
char *deleteChar(const char*, char);
int count_char(const char*, char);
void printComplex(Complex);
void printVector(const Complex*, int);
void printMatrix(Complex**, int);
void insert_char(char* , char, int);

#endif /*UTILS_H*/

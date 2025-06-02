#ifndef DATARECOVERY_H
#define DATARECOVERY_H
#include <stdbool.h>

#include "costants.h"
#include "structure.h"

int getNqbit (char*);
bool getInitVector(char*, Complex*, int);
Complex*** getMatrix(int, const char*, int , char[MAX_MATRIX][MAX_NAME_MATRIX]);
bool getOrderMatrix(const char*, char[MAX_MATRIX][MAX_NAME_MATRIX]);
Complex stringToComplex(const char* );

#endif //DATARECOVERY_H

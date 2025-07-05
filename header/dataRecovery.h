#ifndef DATARECOVERY_H
#define DATARECOVERY_H

#include <stdbool.h>

#include "costants.h"
#include "structure.h"
#include "boolCompact.h"

/* Ottiene il numero di qubits se presenti nella stringa init */
int getNqbit(char*);

/* Estrae il vettore init da una stringa, ritorna true se valido */
bool getInitVector(char*, Complex*, int);

/* Restituisce un array 3D di matrici complesse dato il circuito */
Complex*** getMatrix(int, const char*, int, char[MAX_MATRIX][MAX_NAME_MATRIX]);

/* Ottiene l'ordine delle matrici dal circuito */
bool getOrderMatrix(const char*, char[MAX_MATRIX][MAX_NAME_MATRIX]);

/* Converte una stringa in un numero complesso */
Complex stringToComplex(const char*);

#endif /* DATARECOVERY_H */

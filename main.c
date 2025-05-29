#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "header/structure.h"
#include "header/costants.h"
#include "header/functions.h"

Complex** createMatrix2D(int x, int y) {
    Complex** matrix = malloc(x * sizeof(Complex*));
    if (!matrix) return nullptr;
    for (int i = 0; i<x; i++) {
        matrix[i] = malloc(y * sizeof(Complex*));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return nullptr;
        }
    }
    return matrix;
}

Complex** matrixMoltiplication(Complex ** complex1, Complex ** complex2, const int dim) {
    printf("CIAO");
    Complex** ris = createMatrix2D(dim,dim);

    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            Complex sum = {0,0};
            for(int k = 0; k < dim; k++) {
                Complex temp = mul(&complex1[i][k], &complex2[k][j]);
                sum = add(&sum, &temp);
            }
            ris[i][j] = sum;
        }
    }

    return ris;
}

void freeMatrix2D(Complex** matrix, int dim) {
    if (!matrix) return;

    for (int i = 0; i < dim; i++) { if (matrix[i]) free(matrix[i]); }
    free(matrix);
}

void printMatrix(Complex **matrix, const int dim) {
    for (int j = 0; j<dim; j++) {
        printf("[");
        for (int k = 0; k<dim; k++) {
            printf(" (%d) (%d) Re: %.4f, img: %.4f,", j, k, matrix[j][k].real, matrix[j][k].img);
        }
        printf(" ]\n");
    }
}

int main() {
    char *circ = readFile("../data/circ-ex.txt");
    char *init = readFile("../data/init-ex.txt");

    if (circ != NULL && init != NULL) {
        printf("%s\n", circ);
        printf("%s\n", init);

        const int nQubit = getNqbit(init);

        if (nQubit != 0) {
            printf("numero Qubits: %d\n", nQubit);

            const int dim = (int)pow(2, nQubit);

            Complex *initVector = malloc( dim * sizeof(Complex) );

            if (getInitVector(init, initVector, dim)) {
                for (int i = 0; i<dim; i++) {
                    printf("Complesso%d: img: %.2f, reale: %.2f \n", i, initVector[i].img, initVector[i].real);
                }

                char order[MAX_MATRIX][MAX_NAME_MATRIX] = {0};
                if (getOrderMatrix(circ, order)) {

                    int nMatrix = 0;
                    //conto tutte le stringhe valide
                    for (int i = 0; i<MAX_MATRIX; i++) {
                        if (order[i][0] == '\0') break;
                        nMatrix++;
                    }

                    Complex*** circuit = getMatrix(dim, circ, nMatrix, order);
                    if (circuit != NULL) {

                        for (int i = 0; i<nMatrix; i++) {
                            printf("MATRICE : %s\n", order[i]);
                            printMatrix(circuit[i], dim);
                        }

                        //la inizializzo una matrice identita'
                        Complex** ris = createMatrix2D(dim, dim);
                        for (int i = 0; i<dim; i++) {
                            for (int j = 0; j<dim; j++) {
                                ris[i][j] = (i==j) ? (Complex){1,0} : (Complex){0,0};
                            }
                        }

                        for (int i = 0; i<nMatrix; i++) {
                            printf("HEY");
                            Complex** temp = matrixMoltiplication(ris, circuit[i], dim);
                            //printMatrix(ris,dim);
                            freeMatrix2D(ris, dim);
                            ris = temp;
                        }

                        printf("MOLTIPLICAZIONE TRA MATRICI:\n");
                        printMatrix(ris, dim);

                        freeMatrix2D(ris, dim);

                    }else { printf("ERRORE NELLA DICHIARAZIONE DELLE MATRCICI DI CIRCUITO\n"); }
                    freeMatrix3D(circuit, nMatrix, dim);
                }else { printf("ERRORE NELLA DICHIARAZIONE NOMI CIRCUITI\n"); }
            }else { printf("I DATI INIT NON SONO VALIDI O NON PRESENTI NEL FILE\n"); }
            free(initVector);
        }else { printf("I QUBITS NON SONO VALIDI O NON PRESENTI NEL FILE\n"); }
    }else {
        printf("ERRORE LETTURA FILE\n");
    }

    return 0;
}



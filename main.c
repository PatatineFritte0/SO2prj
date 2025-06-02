#include <complex.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dataRecovery.h"
#include "fileManagement.h"
#include "matrixOperation.h"
#include "utils.h"
#include "header/structure.h"
#include "header/costants.h"

int main() {
    //cerco i file nella directory del programma
    char *circ = readFile("./data/circ.txt");
    char *init = readFile("./data/init.txt");

    if (circ != NULL && init != NULL) {
        //printf("%s\n", circ);
        //printf("%s\n", init);

        //ottengo i qubits
        const int nQubit = getNqbit(init);

        if (nQubit > 0) {
            printf("numero Qubits: %d\n\n", nQubit);
            //se i qubits esistono e sono > 0 allora definisco la
            //dimensione delle matrici e vettori da qui in avanti
            const int dim = (int)pow(2, nQubit);

            //inizializzo il vettore di init
            Complex* initVector = malloc( dim * sizeof(Complex) );

            //controllo se esiste il vettore init e setto il valore in initVector
            if (getInitVector(init, initVector, dim)) {
                printf("VETTORE INIT:\n");
                printVector(initVector, dim);
                printf("\n");

                //inizializzo order imponendo un massimo di
                //matrici che puoi dichiarare con il nome annesso
                char order[MAX_MATRIX][MAX_NAME_MATRIX] = {0};
                if (getOrderMatrix(circ, order)) {

                    int nMatrix = 0;
                    //conto tutte le stringhe valide
                    for (int i = 0; i<MAX_MATRIX; i++) {
                        if (order[i][0] == '\0') break;
                        nMatrix++;
                    }

                    //inizializzo circuit come un array3D dove
                    //1 dim: sono l'ordine di raccorta delle matrici a seconda
                    //della disposizione di order
                    //2-3 dim: sono righe e colonne delle matrici
                    Complex*** circuit = getMatrix(dim, circ, nMatrix, order);
                    if (circuit != NULL) {

                        for (int i = nMatrix-1; i>=0; i--) {
                            printf("MATRICE : %s\n", order[i]);
                            printMatrix(circuit[i], dim);
                        }

                        //inizializzo una matrice identita'
                        //essenziale per la moltiplicazione
                        Complex** mulMatrix = createMatrix2D(dim, dim);
                        for (int i = 0; i<dim; i++) {
                            for (int j = 0; j<dim; j++) {
                                mulMatrix[i][j] = (i==j) ? (Complex){1,0} : (Complex){0,0};
                            }
                        }

                        //faccio la moltiplicazione fra le matrici trovate in ordine inverso
                        for (int i = nMatrix-1; i>=0; i--) {
                            Complex** temp = matrixMoltiplication(mulMatrix, circuit[i], dim);
                            freeMatrix2D(mulMatrix, dim);
                            mulMatrix = temp;
                        }

                        printf("MOLTIPLICAZIONE TRA MATRICI:\n");
                        printMatrix(mulMatrix, dim);

                        printf("\nMOLTIPLICAZIONE TRA LA MATRICE MOLTIPLICATA CON IL VETTORE INIT\n");
                        //moltiplico il risultato della moltiplicazione per il vettore init
                        Complex* result = mulMatrixByVector(mulMatrix, initVector, dim);
                        printVector(result, dim);

                        printf("\nCONTROLLO SULLA CORRETTEZZA: ");

                        //controllo se il risultato sia corretto o meno
                        if (isVectorCorrect(result, dim)) {
                            printf("CORRETTO\n");
                        }
                        else { fprintf(stderr, "NON CORRETTO\n"); }

                        freeMatrix2D(mulMatrix, dim);
                        free(result);
                    }else { fprintf(stderr ,"ERRORE NELLA DICHIARAZIONE DELLE MATRCICI DI CIRCUITO\n"); }
                    freeMatrix3D(circuit, nMatrix, dim);
                }else { fprintf(stderr, "ERRORE NELLA DICHIARAZIONE NOMI CIRCUITI\n"); }
            }else { fprintf(stderr, "I DATI INIT NON SONO VALIDI O NON PRESENTI NEL FILE\n"); }
            free(initVector);
        }else { fprintf(stderr, "I QUBITS NON SONO VALIDI O NON PRESENTI NEL FILE\n"); }
    }else {
        fprintf(stderr, "ERRORE LETTURA FILE\n");
    }

    return 0;
}



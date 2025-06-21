#include <complex.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dataRecovery.h"
#include "fileManagement.h"
#include "matrixOperation.h"
#include "utils.h"
#include "header/structure.h"
#include "header/costants.h"


//è possibile migliorare il debug decommentando le linee di codice di print


int main() {

    char initFile[MAX_CHARACTER_INPUT];
    char circFile[MAX_CHARACTER_INPUT];

    printf(" I DATI DEVONO ESSERE NELLA CARTELLA \"DATA\"\n\n");

    printf(" Fornire il nome del file circ: ");
    scanf("%1024s", circFile);

    printf("Fornire il nome del file init: ");
    scanf("%1024s", initFile);

    // Calcolo la lunghezza necessaria per i path completi
    size_t allocationSize = strlen(INITIAL_FOLDER) + MAX_CHARACTER_INPUT + 1; // +1 per '\0'

    // Alloco e costruisco i path
    char *initPath = malloc(allocationSize);
    char *circPath = malloc(allocationSize);

    if (!initPath || !circPath) {
        fprintf(stderr, "Errore di allocazione\n");
        free(initPath);
        free(circPath);
        return 1;
    }

    strcpy(initPath, INITIAL_FOLDER);
    strcat(initPath, initFile);

    strcpy(circPath, INITIAL_FOLDER);
    strcat(circPath, circFile);

    //cerco i file nella directory del programma
    char *circ = readFile(circPath);
    char *init = readFile(initPath);

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
                //printf("VETTORE INIT:\n");
                //printVector(initVector, dim);
                //printf("\n");

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

                        /*
                        for (int i = nMatrix-1; i>=0; i--) {
                            printf("MATRICE : %s\n", order[i]);
                            printMatrix(circuit[i], dim);
                        }
                        */

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

                        //printf("MOLTIPLICAZIONE TRA MATRICI:\n");
                        //printMatrix(mulMatrix, dim);

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

    free(circPath);
    free(initPath);
    return 0;
}



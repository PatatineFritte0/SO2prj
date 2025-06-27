#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

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
            //printf("numero Qubits: %d\n", nQubit);
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
                    //della disposizione di order (nMatrix)
                    //2-3 dim: sono righe e colonne delle matrici (dim)
                    Complex*** circuit = getMatrix(dim, circ, nMatrix, order);
                    if (circuit != NULL) {

                        /*
                        for (int i = nMatrix-1; i>=0; i--) {
                            printf("MATRICE : %s\n", order[i]);
                            printMatrix(circuit[i], dim);
                        }*/
                        //controllo se il numero di matrici sia almeno 1
                        if (nMatrix > 1) {
                            //setto un valore massimo di thread che verrebbero utilizzati dato nMatrix
                            int max = (int)floor((double)nMatrix/2);
                            int nThread = 0;

                            printf("scegliere il numero di thread da usare: ");
                            scanf("%d", &nThread);
                            
                            if (nThread < 1 ) nThread = 1;
                            //se il numero in input eccede il massimo lo setto ad esso
                            if (nThread > max) nThread = max;

                            while (nMatrix != 1){
                                //setto il "resto" delle matrici dato il numero di thread
                                int restMatrix = nMatrix - MATRIX_MUL_PARALLEL * nThread;

                                //alloco dinamicamente i parametri dei thread
                                struct argThreadMatrMoltiplication *args = malloc (sizeof(struct argThreadMatrMoltiplication) * nThread);

                                //inizializzo una variabile che mi andra a contare gli indici di matrici del buffer totale delle matrici
                                //il buffer e' circuit in questo caso
                                int indexMatrix = 0;
                                for (int i = 0; i<nThread; i++) {
                                    //setto la competenza del thread, che sarebbe quante matrici deve moltiplicare in sequenza
                                    args[i].competence = MATRIX_MUL_PARALLEL;
                                    args[i].matrix = circuit;
                                    //setto da quale indice iniziare a moltiplicare
                                    args[i].indexInit = indexMatrix;
                                    //dimensione della matrice dim*dim
                                    args[i].dimMatrix = dim;

                                    //incremento l'indice
                                    indexMatrix += MATRIX_MUL_PARALLEL;
                                }
                                
                                pthread_t threads[nThread];
                                
                                //avvio tutti i thread
                                for (int i = 0; i<nThread; i++){
                                    if (pthread_create(&threads[i], NULL, mulMatrixThreadFunction, &args[i]) != 0) {
                                        fprintf(stderr, "Errore nella creazione del thread %d\n", i);
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                //creo un vettore di matrici, il vettore ha dimensione pari a
                                //tutti i risultati dei thread + il resto delle matrici
                                Complex*** ris = createMatrix3D(nThread + restMatrix, dim, dim);
                                
                                 //metto nei risultati i resti
                                for (int i = nThread; i<nThread + restMatrix; i++) {
                                    copyMatrix(ris[i], circuit[indexMatrix++], dim, dim);
                                }
                                
                               //ottengo tutti i risultati dai thread
                                for (int i = 0; i<nThread; i++) {
                                    void *retVal;
                                    pthread_join(threads[i], &retVal);
                                    ris[i] = (Complex**) retVal;
                                }

                                //ris diventara' il nuovo buffer di matrici quindi
                                //libero la memoria non piu utilizzata e setto tutti i dati utili
                                //sulla base di ris
                                freeMatrix3D(circuit, nMatrix, dim);
                                nMatrix = nThread + restMatrix;
                                circuit = ris;

                                // Calcola il numero massimo possibile di thread che rispettano la condizione
                                int maxUsableThread = nMatrix / MATRIX_MUL_PARALLEL;
                                if (maxUsableThread < 1) maxUsableThread = 1;

                                // Mantieni il numero di thread più basso tra quello già in uso e il massimo valido
                                if (nThread > maxUsableThread) {
                                    nThread = maxUsableThread;
                                }

                                free(args);
                            }


                        }else {
                            //e' inutile fare dei thread ed inizializzare dati che non verranno utilizzati
                            printf("NUMERO DI CIRCUITI = 1, MOLTIPLICAZIONE SALTATA\n");
                        }

                        Complex **mulMatrixT = createMatrix2D(dim, dim);
                        //il risultato sara' sicuramente nella prima posizione del buffer
                        copyMatrix(mulMatrixT, circuit[0], dim, dim);

                        //printf("MOLTIPLICAZIONE TRA MATRICI:\n");
                        //printMatrix(mulMatrixT, dim);

                        printf("\nMOLTIPLICAZIONE TRA LA MATRICE MOLTIPLICATA CON IL VETTORE INIT\n");
                        //moltiplico il risultato della moltiplicazione per il vettore init
                        Complex* resultT = mulMatrixByVector(mulMatrixT, initVector, dim);
                        printVector(resultT, dim);


                        printf("\nCONTROLLO SULLA CORRETTEZZA: ");

                        //controllo se il risultato sia corretto o meno

                        if (isVectorCorrect(resultT, dim)) {
                            printf("CORRETTO\n");
                        }
                        else { fprintf(stderr, "NON CORRETTO\n"); }
                        freeMatrix2D(mulMatrixT, dim);
                        free(resultT);
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



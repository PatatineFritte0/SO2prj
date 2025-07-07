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

/* è possibile migliorare il debug decommentando le linee di codice di print */

int main(int argc, char *argv[]) {
    
    if(argc < 4){
      fprintf(stderr, "FILE INPUT INSUFFICIENTI\n");
      exit(0);
    }
    
    if(argc > 4){
      printf("uso solo i primi 3 parametri in input: initFile, circFile, nThread\n");
    }

    /*Dichiaro tutte le veriabili utili*/
    char initFile[MAX_CHARACTER_INPUT];
    char circFile[MAX_CHARACTER_INPUT];

    char *initPath;
    char *circPath;
    size_t allocationSize;
    
    strcpy(initFile, argv[1]); /* nome del file init */
    strcpy(circFile, argv[2]); /* nome del file circ */

    int nQubit;
    int dim; /*dimensione dei vettori e matrici*/
    Complex *initVector; /*vettore init*/

    char order[MAX_MATRIX][MAX_NAME_MATRIX];
    int nMatrix; /*numero di matrici presente in circuit*/
    Complex ***circuit; /*vettore di matrici ordinate da order*/

    int i; /*indice utile nei cicli for*/
    int max; /*massimo di thread utili*/
    
    i=0;
    while(argv[3][i] != '\0'){
      if(!isdigit(argv[3][i++])){
        fprintf(stderr, "il numero di thread non e' un numero\n");
        exit(0);
      }
    }
    
    int nThread = atoi(argv[3]); /*numero di thread in input mssimo utile*/
    
    
    int restMatrix; /*matrici  che non vengono inserite nei  thread*/
    int indexMatrix; /*indice per capire dove si e' nello scorrimento di circuit*/

    struct argThreadMatrMoltiplication *args; /*argomento dei threads*/
    pthread_t *threads;  /*i threads*/
    Complex ***ris; /*vettore di threads momentaneo*/
    Complex **mulMatrixT; /*il risultato della moltiplicazione dei threads*/
    Complex *resultT; /*vettore fin*/

    printf("I DATI DEVONO ESSERE NELLA CARTELLA \"DATA\"\n\n");

    allocationSize = strlen(INITIAL_FOLDER) + MAX_CHARACTER_INPUT + 1; /* +1 per '\0' */

    initPath = (char *) malloc(allocationSize);
    circPath = (char *) malloc(allocationSize);

    if (!initPath || !circPath) {
        fprintf(stderr, "Errore di allocazione\n");
        free(initPath);
        free(circPath);
        return 1;
    }

    strcpy(initPath, INITIAL_FOLDER);
    strcat(initPath, initFile);
    
    /*creo la path*/
    
    strcpy(circPath, INITIAL_FOLDER);
    strcat(circPath, circFile);

    char *circ = readFile(circPath);
    char *init = readFile(initPath);

    if (circ != NULL && init != NULL) {
    
        /*ottengo i qubit*/
        nQubit = getNqbit(init);

        if (nQubit > 0) {
            dim = (int) pow(2.0, (double) nQubit);

            initVector = (Complex *) malloc(dim * sizeof(Complex));
            if (initVector == NULL) {
                fprintf(stderr, "Errore di allocazione per initVector\n");
                free(circ);
                free(init);
                free(initPath);
                free(circPath);
                return 1;
            }
            /*ottengo il vettore init*/
            if (getInitVector(init, initVector, dim)) {
                /*inizializzo order*/
                for (i = 0; i < MAX_MATRIX; i++) {
                    int j;
                    for (j = 0; j < MAX_NAME_MATRIX; j++) {
                        order[i][j] = 0;
                    }
                }
                /*ottengo l'ordine di matrici e quindi le matrici utili*/
                if (getOrderMatrix(circ, order)) {
                    nMatrix = 0;
                    for (i = 0; i < MAX_MATRIX; i++) {
                        if (order[i][0] == '\0') break;
                        nMatrix++; /*le matrici utili*/
                    }
                    /*ottengo  le matrici utili*/
                    circuit = getMatrix(dim, circ, nMatrix, order);
                    if (circuit != NULL) {
                        if (nMatrix > 1) {
                            /*capisco quanti threads mi servono*/
                            max = (int) floor((double) nMatrix / 2.0);
                            
                            /*se inserisce un dato non vali lo rendo valido*/
                            if (nThread < 1) nThread = 1;
                            if (nThread > max) nThread = max;

                            while (nMatrix != 1) {
                                /*capisco quante matrici di resto avro' dal numero di threads*/
                                restMatrix = nMatrix - MATRIX_MUL_PARALLEL * nThread;
                                
                                args = (struct argThreadMatrMoltiplication *) malloc(sizeof(struct argThreadMatrMoltiplication) * nThread);
                                threads = (pthread_t *) malloc(sizeof(pthread_t) * nThread);

                                indexMatrix = 0;
                                /*inizializzo arg dei threads*/
                                for (i = 0; i < nThread; i++) {
                                    args[i].competence = MATRIX_MUL_PARALLEL;
                                    args[i].matrix = circuit;
                                    args[i].indexInit = indexMatrix;
                                    args[i].dimMatrix = dim;
                                    indexMatrix += MATRIX_MUL_PARALLEL;
                                }

                                for (i = 0; i < nThread; i++) {
                                    /*creo i threads*/
                                    if (pthread_create(&threads[i], NULL, mulMatrixThreadFunction, (void *) &args[i]) != 0) {
                                        fprintf(stderr, "Errore nella creazione del thread %d\n", i);
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                /*creo la matrice ris*/
                                ris = createMatrix3D(nThread + restMatrix, dim, dim);
                                /*nel mentre che i thread finiscono le matrici di resto in ris*/
                                for (i = nThread; i < nThread + restMatrix; i++) {
                                    copyMatrix(ris[i], circuit[indexMatrix++], dim, dim);
                                }
                                
                                /*inserisco in ris il risultato dei threads*/
                                for (i = 0; i < nThread; i++) {
                                    void *retVal;
                                    pthread_join(threads[i], &retVal);
                                    ris[i] = (Complex **) retVal;
                                }

                                freeMatrix3D(circuit, nMatrix, dim);
                                /*aggiorno le variabili utili e calcolo i threads utili*/
                                nMatrix = nThread + restMatrix;
                                circuit = ris;

                                max = nMatrix / MATRIX_MUL_PARALLEL;
                                if (max < 1) max = 1;
                                if (nThread > max) nThread = max;

                                free(args);
                                free(threads);
                            }
                        } else {
                            printf("NUMERO DI CIRCUITI = 1, MOLTIPLICAZIONE SALTATA\n");
                        }
                        
                        /*inserisco in mulmatrix il  risultato dell operazione de fata*/
                        mulMatrixT = createMatrix2D(dim, dim);
                        copyMatrix(mulMatrixT, circuit[0], dim, dim);

                        printf("\nMOLTIPLICAZIONE TRA LA MATRICE MOLTIPLICATA CON IL VETTORE INIT\n");
                        /*faccio la moltiplicazione e stampo il risultato*/
                        resultT = mulMatrixByVector(mulMatrixT, initVector, dim);
                        printVector(resultT, dim);

                        printf("\nCONTROLLO SULLA CORRETTEZZA: ");
                        fflush(stdout);
                        /*controllo che tutto sia corretto*/
                        if (isVectorCorrect(resultT, dim)) {
                            printf("CORRETTO\n");
                        } else {
                            fprintf(stderr, "NON CORRETTO\n");
                        }

                        freeMatrix2D(mulMatrixT, dim);
                        free(resultT);

                        freeMatrix3D(circuit, nMatrix, dim);
                    } else {
                        fprintf(stderr, "ERRORE NELLA DICHIARAZIONE DELLE MATRCICI DI CIRCUITO\n");
                    }
                } else {
                    fprintf(stderr, "ERRORE NELLA DICHIARAZIONE NOMI CIRCUITI\n");
                }
            } else {
                fprintf(stderr, "I DATI INIT NON SONO VALIDI O NON PRESENTI NEL FILE\n");
            }
            free(initVector);
        } else {
            fprintf(stderr, "I QUBITS NON SONO VALIDI O NON PRESENTI NEL FILE\n");
        }
        free(circ);
        free(init);
    } else {
        fprintf(stderr, "ERRORE LETTURA FILE\n");
    }

    free(circPath);
    free(initPath);

    return 0;
}


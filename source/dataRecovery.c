#include "../header/dataRecovery.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/utils.h"
#include "../header/matrixOperation.h"

/* questa funzione ottiene il numero di qubits se sono presenti in init */
int getNqbit (char* init){
    int indexBuffer = 0, indexInit = 0;
    char buffer[100] = "\0";
    /* controllo se il comando e' in init */
    while (indexBuffer == 0 && isPresent("#qubits", init, &indexInit)) {
        /* salto gli spazi fra lui ed il dato */
        while (init[indexInit] == ' ' || init[indexInit] == '\t' || init[indexInit] == '\n'){ indexInit++; }
        /* fin quando e' un numero, lo copio nel buffer */
        while (isdigit(init[indexInit]) && indexBuffer < 100) {
            buffer[indexBuffer++] = init[indexInit++];
        }
    }
    buffer[indexBuffer] = '\0';

    /* controllo se sono stati trovati dati validi */
    if (indexBuffer == 0) {
        return 0;
    }

    return atoi(buffer);
}

/* questa funzione cerca di ottenere il vettore init (true)
   in caso contrario restituisce false */
bool getInitVector(char* init, Complex* vector, int dimension) {
    int indexInit = 0;
    int nFindNumbers = 0;

    while (isPresent("#init", init, &indexInit)) {
        char buffer[100];
        int indexBuffer = 0;

        /* salto tutti i caratteri che non mi interessa analizzare */
        while (init[indexInit] == ' ' || init[indexInit] == '\t' || init[indexInit] == '\n' || init[indexInit] == '\r') {
            indexInit++;
        }

        /* se trovo un apertura di vettore */
        if (init[indexInit] == '[') {
            indexInit++; /* salta '[' */

            while (init[indexInit] != '\0') {
                char ch = init[indexInit];

                /* controllo se la sintassi sia corretta */
                if (ch == '+' || ch == '-' || ch == 'i' || ch == '.' || isdigit(ch)) {
                    buffer[indexBuffer++] = ch;
                } else if (ch == ',' || ch == ']') {
                    /* trovo la fine di un numero */
                    buffer[indexBuffer] = '\0';

                    if (nFindNumbers >= dimension) break;
                    /* se non ne ho trovati troppi */
                    if (count_char(buffer, 'i') <= 1) {
                        /* inserisco nel vettore risultante */
                        vector[nFindNumbers++] = stringToComplex(buffer);
                    } else {
                        /* il vettore non e' valido */
                        nFindNumbers = 0;
                        break;
                    }

                    indexBuffer = 0;
                    /* finisco il ciclo non appena trovo la fine di un vettore */
                    if (ch == ']') break;
                } else if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
                    /* ignora */
                } else {
                    /* ho trovato un carattere illegale */
                    nFindNumbers = 0;
                    break;
                }

                indexInit++;
            }
            /* se il dato e' valido */
            if (nFindNumbers == dimension) {
                return true;
            } else {
                nFindNumbers = 0;
            }
        }
    }
    /* non ho trovato nessun init valido */
    return false;
}

/* questa funzione restituisce un numero complesso da una stringa */
Complex stringToComplex(const char* string) {
    /* inizializzo un numero complesso a 0 */
    Complex result = {0.0, 0.0};

    /* inizializzo e copio la stringa sulla quale andro' a lavorare */
    char input[256] = "";
    strncpy(input, string, sizeof(input));
    input[sizeof(input) - 1] = '\0';

    /* controllo se e' reale senza parte immaginaria */
    char* i_ptr = strchr(input, 'i');
    if (!i_ptr) {
        result.real = atof(input);
        return result;
    }

    *i_ptr = '\0';  /* Rimuove la 'i' all indirizzo della stringa e lo rimpiazza con \0 */

    /* Se è solo "i", "+i" o "-i" */
    if (strcmp(input, "") == 0) {
        result.img = 1.0;
        return result;
    } else if (strcmp(input, "+") == 0) {
        result.img = 1.0;
        return result;
    } else if (strcmp(input, "-") == 0) {
        result.img = -1.0;
        return result;
    }

    *i_ptr = 'i'; /* reinserisco la 'i' che mi servira' a capire quale parte e' immaginaria o meno */

    /* Cerca il segno che separa reale e immaginaria
       (si da' per scontato che alla fine di un numero non ci puo' essere un segno) */
    int sep = 0;
    {
        int i;
        for (i = (int)strlen(input) - 1; i > 0; i--) {
            if (input[i] == '+' || input[i] == '-') {
                sep = i;
                break;
            }
        }
    }

    if (sep != 0) { /* ha trovato un segno che separa le due parti */
        char imag[256] = "", real[256] = "";
        strncpy(imag, input, sep);
        imag[sep] = '\0';
        /* copia la stringa dal + o - */
        strcpy(real, &input[sep]);

        /* Determina chi è reale e chi immaginario */
        if (strchr(imag, 'i')) {
            char* clearImg = deleteChar(imag, 'i');
            result.img = atof(clearImg);
            result.real = atof(real);
            free(clearImg);
        } else {
            char* clearReal = deleteChar(real, 'i');
            /* ho i, ma non il coefficente */
            if (strcmp(clearReal, "-") == 0 || strcmp(clearReal, "+") == 0) {
                insert_char(clearReal, '1', 1);
            }

            result.img = atof(clearReal);
            result.real = atof(imag);
            free(clearReal);
        }
    } else { /* significa che la parte reale non c'e'
              la 'i' aveva un coefficiente */
        result.img = atof(input);
    }

    return result;
}

/* questa funzione cerca di ottenere l'ordine di matrici del circuito */
bool getOrderMatrix(const char* circ, char order[MAX_MATRIX][MAX_NAME_MATRIX]){
    int indexOrder = 0, indexName = 0;
    int indexCirc = 0;

    /* booleana che mi indica quando il valore trovato è valido o meno */
    bool notValid = true;

    /* continua fun quando trova un comando circ oppure trova un valore valido */
    while ( notValid && isPresent("#circ", circ, &indexCirc) && circ[indexCirc] != '\0') {
        /* salto gli spazi */
        while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') { indexCirc++; }
        /* controllo se il valore trovao non e' un accapo oppure fine file e che non si ecceda il limitre di matrice */
        while (circ[indexCirc] != '\n' && circ[indexCirc] != '\0' && indexOrder < MAX_MATRIX ) {
            notValid = false;
            while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') { indexCirc++; }
            /* se c'e' scritto qualcosa */
            if (circ[indexCirc] != '\n' &&circ[indexCirc] != '\0') {
                /* per ogni cosa scritta copio nella matrice dei nomi in ordine di ritrovamento */
                while (circ[indexCirc] != ' ' && circ[indexCirc] != '\n' && circ[indexCirc] != '\0' && circ[indexCirc] != '\t' && indexName < MAX_NAME_MATRIX) {
                    order[indexOrder][indexName++] = circ[indexCirc++];
                }
                /* prossimo nome di circuito */
                indexOrder++;
                indexName = 0;
            }
        }
    }
    /* se non trovi nulla */
    if (indexOrder == 0) {
        return false;
    }

    return true;
}

/* questa funzione cerca di ottenere un vettore di matrici dato l'ordine con cui sono
   state dichiarate */
Complex*** getMatrix(const int dimention, const char* circ, const int nMatrix , char order[MAX_MATRIX][MAX_NAME_MATRIX]) {
    /* inizializzo la matrice di ritorno */
    Complex*** orderMatrix = createMatrix3D(nMatrix,dimention,dimention);
    if (!orderMatrix) return NULL;

    int indexOrder = 0, indexCirc = 0;
    /* per ogni define trovato fin quando non eccedo il numero di matrici che mi interessano */
    while (isPresent("#define", circ, &indexCirc) && indexOrder < nMatrix) {
        /* inizializzo il buffer */
        int indexBuffer = 0;
        char bufferName[MAX_NAME_MATRIX] = {0};
        /* salto gli spazi */
        while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') { indexCirc++; }

        /* copio il nome */
        while ( circ[indexCirc] != ' ' && circ[indexCirc] != '\n' && circ[indexCirc] != '\0' && circ[indexCirc] != '\t' ) {
            bufferName[indexBuffer++] = circ[indexCirc++];
        }

        /* controllo se e' quello che mi interessa */
        if (strcmp(order[indexOrder], bufferName) == 0) {
            /* booleana che mi controlla se i dati sono validi */
            bool isValid = true;
            while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t' || circ[indexCirc] == '\n') { indexCirc++; }
            /* controllo se inizia una marice */
            if (circ[indexCirc++] == '[') {
                /* inizializzo un buffer per le operazioni */
                char bufferOperation[MAX_NAME_MATRIX] = {0};
                int indexOperationBuffer = 0;
                /* inizializzo le righe e colonne per inserire i valori nella matrice */
                int row = 0, column = 0;

                /* fin quando la matrice non si chiude */
                while (circ[indexCirc] != ']' && isValid) {
                    if (circ[indexCirc] == ')') indexCirc++;
                    /* inizializzo per ogni ')' le colonne a 0 */
                    column = 0;
                    while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t' || circ[indexCirc] == '\n') { indexCirc++; }

                    /* se inizia la riga */
                    if (circ[indexCirc] == '(') {
                        /* fin quando non finisce la riga */
                        while (circ[indexCirc] != ')' && isValid) {
                            indexCirc++;

                            while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t' || circ[indexCirc] == '\n') { indexCirc++; }

                            /* controllo se i caratteri sono legali per l'inserimaneto in matrice */
                            if (circ[indexCirc] == '+' || circ[indexCirc] == '-' || circ[indexCirc] == 'i' || circ[indexCirc] == '.' || isdigit(circ[indexCirc])) {
                                bufferOperation[indexOperationBuffer++] = circ[indexCirc];
                            /* se hai preso correttamente il dato */
                            } else if ((circ[indexCirc] == ',' || circ[indexCirc] == ')') && bufferOperation[0] != '\0'){
                                /* controlla se le dimensioni siano rispettate */
                                if (column < dimention && row < dimention) {
                                    /* printf("%.4f\n%s\n", stringToComplex(bufferOperation).img, bufferOperation); */

                                    /* inserisci il dato in matrice come numero complesso */
                                    orderMatrix[indexOrder][row][column] = stringToComplex(bufferOperation);

                                    /* printf("Re: %.4f, img: %.4f \n", orderMatrix[indexOrder][row][column].real, orderMatrix[indexOrder][row][column].img); */

                                    /* ripulisci il buffer */
                                    memset(&bufferOperation, 0, sizeof(bufferOperation));
                                    indexOperationBuffer = 0;
                                }
                                column++;
                                /* controlli se e' la fine di una riga */
                                if (circ[indexCirc] == ')') {
                                    /* controlli che le dimensioni siano rispettate */
                                    if (dimention != column) isValid = false;
                                }
                            } else {
                                /* dato non valido */
                                isValid = false;
                                break;
                            }
                        }

                        indexCirc++;

                    } else if (circ[indexCirc] == ']') {
                        row--; /* è una pezza, spero non si noti */
                    } else {
                        /* dato non valido */
                        printf("\n%c\n", circ[indexCirc]);
                        isValid = false;
                        break;
                    }
                    /* vai alla prossima riga */
                    row++;
                }
                /* controlli se le dimensioni sono rispettate */
                if (dimention != row) isValid = false;
            }

            /* se il dato è valido allora puoi andare con la prossima matrice */
            if (isValid) {
                indexOrder++;
                /* si riinizializza a 0 l'indice di ricerca cosi da poter ricontrollare
                   quello che aveva gia passato */
                indexCirc = 0;
            }
        }
    }
    /* se non sono state inserite tutte le matrici nella matrice */
    if (indexOrder != nMatrix) {
        /* libero la memoria */
        freeMatrix3D(orderMatrix, nMatrix, dimention);
        return NULL;
    }

    return orderMatrix;
}


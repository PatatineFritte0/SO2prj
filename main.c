#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "function.h"
#include <math.h>

int count_char(const char* str, char target) {
    int count = 0;
    while (*str) {
        if (*str == target) count++;
        str++;
    }
    return count;
}

char *deleteChar(const char *string, const char ch) {
    size_t len = strlen(string);
    char *newString = malloc(len + 1); // +1 per il terminatore null
    if (!newString) return NULL; // gestione fallimento allocazione

    int j = 0;
    for (size_t i = 0; i < len; i++) {
        if (string[i] != ch) {
            newString[j++] = string[i];
        }
    }
    newString[j] = '\0';
    return newString;
}

Complex stringToComplex(const char* string) {
    Complex result = {0.0, 0.0};

    //inizializzo e copio la stringa sulla quale andro' a lavorare
    char input[256] = "";
    strncpy(input, string, sizeof(input));
    input[sizeof(input) - 1] = '\0';

    //controllo se e' reale senza parte immaginaria
    char* i_ptr = strchr(input, 'i');
    if (!i_ptr) {
        result.real = atof(input);
        return result;
    }

    *i_ptr = '\0';  // Rimuove la 'i' all indirizzo della stringa e lo rimpiazza con \0

    // Se è solo "i", "+i" o "-i"
    if (strcmp(input, "") == 0) {
        result.img = 1.0;
        return result;
    }else if (strcmp(input, "+") == 0) {
        result.img = 1.0;
        return result;
    } else if (strcmp(input, "-") == 0) {
        result.img = -1.0;
        return result;
    }

    *i_ptr = 'i';

    // Cerca il segno che separa reale e immaginaria
    int sep = 0;
    for (int i = (int)strlen(input) - 1; i > 0; i--) {
        if (input[i] == '+' || input[i] == '-') {
            sep = i;
            break;
        }
    }

    if (sep) {
        char imag[256] ="", real[256] ="";
        strncpy(imag, input, sep);
        imag[sep] = '\0';
        //copia la stringa dal + o -
        strcpy(real, &input[sep]);

        // Determina chi è reale e chi immaginario
        if (strchr(imag, 'i')) {
            char* clearImg = deleteChar(imag, 'i');
            result.img = atof(clearImg);
            result.real = atof(real);
            free(clearImg);
        } else {
            char* clearReal = deleteChar(real, 'i');
            result.real = atof(imag);
            result.img = atof(clearReal);
            free(clearReal);
        }
    } else {
        result.img = atof(input);
    }

    return result;
}

bool getInitVector(char* init, Complex* vector) {
    int indexBuffer = 0, indexInit = 0;
    char buffer[100] = "\0";
    //controllo se il comando e' in init

    int nFindNumbers = 0;
    while (indexBuffer == 0 && isPresent("#init", init, &indexInit)) {
        memset(buffer, '\0', sizeof(buffer));
        buffer[0] = '\0';
        // salto gli spazi fra lui ed il dato
        while (init[indexInit] == ' ' || init[indexInit] == '\t'){ indexInit++; }

        if (init[indexInit] == '[') {
            while (init[indexInit] != ']' && init[indexInit] != '\0') {
                indexInit++;

                if(init[indexInit] == '+' || init[indexInit] == '-' || init[indexInit] == 'i' || init[indexInit] == '.' || isdigit(init[indexInit])) {
                    buffer[indexBuffer++] = init[indexInit];
                }else if (init[indexInit] == ',' || init[indexInit] == ']') {

                    buffer[indexBuffer] = '\0';

                    if (count_char(buffer, 'i') <= 1) {
                        vector[nFindNumbers] = (Complex){0,0};
                        vector[nFindNumbers++] = stringToComplex(buffer);
                        //vector[nFindNumbers] = stringToComplex("-5");
                        //printf("Re: %.4f, img: %.4f \n", vector[nFindNumbers].real, vector[nFindNumbers].img);
                        //printf("%s\n", buffer);
                        //nFindNumbers++;
                    } else {
                        // c'e un errore di sintassi
                        buffer[0] = '\0';
                        indexBuffer = 0;
                        nFindNumbers = 0;
                        break;
                    }
                    //riinizializzo il buffer
                    indexBuffer = 0;
                 }else if (init[indexInit] == ' ' || init[indexInit] == '\t') {
                    //niente
                }else {
                    //riconosco che c'e' un errore di sintassi
                    buffer[0] = '\0';
                    indexBuffer = 0;
                    nFindNumbers = 0;
                    break;
                }
            }
        }
    }

    //controllo se sono stati trovati dati validi
    if (buffer[indexBuffer] == '\0') {
        return false;
    }
    return true;
}

int main() {
    char *circ = readFile("../data/circ-ex.txt");
    char *init = readFile("../data/init-ex.txt");
    if (circ != NULL && init != NULL) {
        printf("%s\n", circ);
        printf("%s\n", init);

        int nQubit = getNqbit(init);

        if (nQubit != 0) {
            printf("numero Qubits: %d\n", nQubit);

            double dim = pow(2, nQubit);
            Complex *initVector = malloc( (int)dim * sizeof(Complex) );
            if (getInitVector(init, initVector)) {
                for (int i = 0; i<dim; i++) {
                    printf("Complesso%d: img: %.2f, reale: %.2f \n", i, initVector[i].img, initVector[i].real);
                }



            }else {
                printf("I DATI INIT NON SONO VALIDI O NON PRESENTI NEL FILE\n");
            }
        }else {
            printf("I QUBITS NON SONO VALIDI O NON PRESENTI NEL FILE\n");
        }


    }


    return 0;
}



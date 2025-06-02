#include "../header/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

//questa funzione cerca un comando di inizializzazione e' all interno di una stringa ed oltre a loro chiede
//in input l'indice con il quale inizia a cercare le due stringhe uguali.
//appena lo trova ritorna il valore true e modifica l'indice fino al carattere subito dopo alla stringa trovata
bool isPresent(const char* comparator,const char* string , int *indexInit) {
    //dichiaro i dati dei buffer
    int indexBuffer = 0;
    char buffer[100] = "\0";

    //controllo se la stringa non sia finita
    while (string[*indexInit] != '\0') {
        //vado avanti fino a quando non trova il segnale di inizio comando '#'
        while (string[*indexInit] != '#' && string[*indexInit] != '\0'){ (*indexInit)++; }

        //copio nel buffer tutto quelo che c'e' dopo '#'
        while (string[*indexInit] != '\n' && string[*indexInit] != '\0' && string[*indexInit] != ' ' && indexBuffer < 100){
            buffer[indexBuffer++] = string[(*indexInit)++];
        }
        buffer[indexBuffer] = '\0';

        //controllo se e' lo stesso comando
        if (strcmp(buffer, comparator) == 0) { return true; }
        indexBuffer = 0;
    }

    //printf("ERRORE: PARAMETRO %s NON TROVATO\n", comparator);
    return false;
}

//questa funzione controlla se il vettore è valido o meno
bool isVectorCorrect(const Complex * vector, const int dim) {
    double sum = 0;

    for (int i = 0; i < dim; i++) {
        sum += pow(mod(vector[i]), 2);
    }

    //creo una epsilon che delimita l'errore massimo consentito dalla somma
    //uso constexpr per aumentare la velocità del programma
    const double epsilon = 0.00000000000001;
    if ( fabs(sum - 1) < epsilon) return true;

    return false;
}

//elimina un determinato carattere da una stringa
char *deleteChar(const char *string, const char ch) {
    size_t len = strlen(string);
    char *newString = malloc(len + 1); // +1 per il terminatore null
    if (!newString) return NULL; // gestione fallimento allocazione

    int j = 0;
    //copio la stringa tranne quel carattere
    for (size_t i = 0; i < len; i++) {
        if (string[i] != ch) {
            newString[j++] = string[i];
        }
    }
    newString[j] = '\0';
    return newString;
}

//questa funzione conta un determinato carattere in una stringa
int count_char(const char* str, char target) {
    int count = 0;
    //conta i caratteri interessati fin quando ci sono
    while (*str) {
        if (*str == target) count++;
        str++;
    }
    return count;
}

//questa funzione stampa un numero complesso
void printComplex(Complex com) {
    printf("Re: %.4f, img: %.4f", com.real, com.img);
}

//questa funzione stampa un vettore di numeri complessi
void printVector(const Complex* vector, const int dim) {
    printf("(\n");
    for (int i = 0; i<dim; i++) {
        printf("(%d): ", i);
        printComplex(vector[i]);
        printf("\n");
    }
    printf(")\n");
}

//stampa una matrice 2D di numeri complessi
void printMatrix(Complex **matrix, const int dim) {
    for (int j = 0; j<dim; j++) {
        printf("[");
        for (int k = 0; k<dim; k++) {
            if (k != 0) printf(" || ");
            printf("(%d)-(%d) ", j, k);
            printComplex(matrix[j][k]);
        }
        printf(" ]\n");
    }
}

//questa funzione shifta tutta la stringa a partire da una posizione
//e inserisce il carattere in quella posizione
void insert_char(char *str, const char c, const int pos) {
    const size_t len = strlen(str);

    // Shift verso destra i caratteri, incluso il terminatore '\0'
    for (int i = len; i >= pos; i--) {
        str[i + 1] = str[i];
    }

    // Inserisci il carattere
    str[pos] = c;
}

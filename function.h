#ifndef FUNCTION_H
#define FUNCTION_H
#import "prototype.h"
//----------------------------------------------------------------------------------------------------------
//--------------------------------------FILE MANAGEMENT FUNCTIONS-------------------------------------------
//----------------------------------------------------------------------------------------------------------

char* readFile(const char* directory){
    //apertura file
    FILE* file = fopen(directory, "r");
    if (file == NULL) {
        printf("ERRORE IN APERTURA FILE DI %s\n", directory);
        return NULL;
    }

    //sposto l'indicatore alla fine del file,
    //prendo la grandezza e lo riporto all inizio
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    rewind(file);

    //allocazione memoria per la stringa
    char* content = malloc(size+1);
    if (content == NULL) {
        printf("ERRORE PER L'ALLOCAZIONE IN MEMORIA DI %s\n", directory);
        return NULL;
    }

    //trascrizione da file a buffer
    content[size] = '\0';
    fread(content, 1, size, file);

    //chiudo il file e ritorno il contenuto della stringa
    fclose(file);
    return content;
}

//----------------------------------------------------------------------------------------------------------
//-----------------------------STRING MANIPULATION & DATA RECOVERY FUNCTIONS--------------------------------
//----------------------------------------------------------------------------------------------------------

//cerca di ottenere il numero di qubits se sono presenti in init
int getNqbit (char* init){
    int indexBuffer = 0, indexInit = 0;
    char buffer[100] = "\0";
    //controllo se il comando e' in init
    while (indexBuffer == 0 && isPresent("#qubits", init, &indexInit)) {
        // salto gli spazi fra lui ed il dato
        while (init[indexInit] == ' ' || init[indexInit] == '\t'){ indexInit++; }
        //fin quando e' un numero, lo copio nel buffer
        while (isdigit(init[indexInit]) && indexBuffer < 100) {
            buffer[indexBuffer++] = init[indexInit++];
        }
    }
    buffer[indexBuffer] = '\0';

    //controllo se sono stati trovati dati validi
    if (indexBuffer == 0) {
        return 0;
    }

    return atoi(buffer);
}

//----------------------------------------------------------------------------------------------------------
//--------------------------------------MATRIX OPERATION FUNCTIONS------------------------------------------
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
//--------------------------------------UTILITY FUNCTIONS---------------------------------------------------
//----------------------------------------------------------------------------------------------------------

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

#endif //FUNCTION_H

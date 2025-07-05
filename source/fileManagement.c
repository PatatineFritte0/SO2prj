#include "../header/fileManagement.h"

#include <stdio.h>
#include <stdlib.h>

/* questa funzione restituisce il contenuto del file in una stringa */
char* readFile(const char* directory){
    /* apertura file */
    FILE* file = fopen(directory, "r");
    if (file == NULL) {
        /* printf("ERRORE IN APERTURA FILE DI %s\n", directory); */
        return NULL;
    }

    /* sposto l'indicatore alla fine del file,
       prendo la grandezza e lo riporto all inizio */
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    rewind(file);

    /* allocazione memoria per la stringa */
    char* content = (char*) malloc(size + 1);
    if (content == NULL) {
        /* printf("ERRORE PER L'ALLOCAZIONE IN MEMORIA DI %s\n", directory); */
        fclose(file);
        return NULL;
    }

    /* trascrizione da file a buffer */
    size_t bytesRead = fread(content, 1, size, file);
    content[bytesRead] = '\0';

    /* chiudo il file e ritorno il contenuto della stringa */
    fclose(file);
    return content;
}


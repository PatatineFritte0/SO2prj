#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include <stdbool.h>

#include "../header/functions.h"

#include "../header/costants.h"
#include "../header/structure.h"

//----------------------------------------------------------------------------------------------------------
//-------------------------------------FILE MANAGEMENT FUNCTION---------------------------------------------
//----------------------------------------------------------------------------------------------------------

//questa funzione restituisce il contenuto del file in una stringa
char* readFile(const char* directory){
    //apertura file
    FILE* file = fopen(directory, "r");
    if (file == NULL) {
        //printf("ERRORE IN APERTURA FILE DI %s\n", directory);
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
        //printf("ERRORE PER L'ALLOCAZIONE IN MEMORIA DI %s\n", directory);
        return NULL;
    }

    //trascrizione da file a buffer
    size_t bytesRead = fread(content, 1, size, file);
    content[bytesRead] = '\0';

    //chiudo il file e ritorno il contenuto della stringa
    fclose(file);
    return content;
}

//----------------------------------------------------------------------------------------------------------
//-----------------------------STRING MANIPULATION & DATA RECOVERY FUNCTIONS--------------------------------
//----------------------------------------------------------------------------------------------------------

//questa funzione ottiene il numero di qubits se sono presenti in init
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

//questa funzione cerca di ottenere il vettore init (true)
//in caso contrario restituisce false
bool getInitVector(char* init, Complex* vector, int dimention) {
    int indexBuffer = 0, indexInit = 0;
    char buffer[100] = "\0";


    //controllo se il comando e' in ini
    int nFindNumbers = 0;
    while (indexBuffer == 0 && isPresent("#init", init, &indexInit)) {
        memset(buffer, '\0', sizeof(buffer));
        buffer[0] = '\0';
        // salto gli spazi fra lui ed il dato
        while (init[indexInit] == ' ' || init[indexInit] == '\t'){ indexInit++; }

        //controllo se la sintassi e' giusta
        if (init[indexInit] == '[') {
            while (init[indexInit] != ']' && init[indexInit] != '\0') {
                indexInit++;

                //controllo se i caratteri sono legali per i dati in questione
                if(init[indexInit] == '+' || init[indexInit] == '-' || init[indexInit] == 'i' || init[indexInit] == '.' || isdigit(init[indexInit])) {
                    buffer[indexBuffer++] = init[indexInit];
                }else if (init[indexInit] == ',' || init[indexInit] == ']') { //trovi la fine di un dato
                    buffer[indexBuffer] = '\0';

                    if (nFindNumbers >= dimention) break;
                    if (count_char(buffer, 'i') <= 1) { //mi assicuro che la sintassi sia corretta
                        //inizializzo il vettore e tramuto la stringa in un numero complesso
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
                    //ri inizializzo il buffer
                    indexBuffer = 0;
                 }else if (init[indexInit] == ' ' || init[indexInit] == '\t') {
                    //niente
                }else {
                    //riconosco che c'e' un errore di sintassi
                    buffer[0] = '\0';
                    indexBuffer = 0;
                    nFindNumbers = 0;
                    break; //esco da ciclo e controllo se ci sono altri init validi
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

//questa funzione restituisce un numero complesso da una stringa
Complex stringToComplex(const char* string) {
    //inizializzo un numero complesso a 0
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

    *i_ptr = 'i'; //reinserisco la 'i' che mi servira' a capire quale parte e' immaginaria o meno

    // Cerca il segno che separa reale e immaginaria
    // (si da' per scontato che alla fine di un numero non ci puo' essere un segno)
    int sep = 0;
    for (int i = (int)strlen(input) - 1; i > 0; i--) {
        if (input[i] == '+' || input[i] == '-') {
            sep = i;
            break;
        }
    }


    if (sep != 0) { //ha trovato un segno che separa le due parti
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
            //ho i, ma non il coefficente
            if ( strcmp(clearReal, "-") == 0 || strcmp(clearReal, "+") == 0 ) {
                insert_char(clearReal, '1', 1);
            }

            result.img = atof(clearReal);
            result.real = atof(imag);
            free(clearReal);
        }
    } else { //significa che la parte reale non c'e'
             //la 'i' aveva un coefficiente
        result.img = atof(input);
    }

    return result;
}

//questa funzione cerca di ottenere l'ordine di matrici del circuito
bool getOrderMatrix(const char* circ, char order[MAX_MATRIX][MAX_NAME_MATRIX]){
    int indexOrder = 0, indexName = 0;
    int indexCirc = 0;

    //booleana che mi indica quando il valore trovato è valido o meno
    bool notValid = true;

    //continua fun quando trova un comando circ oppure trova un valore valido
    while ( notValid && isPresent("#circ", circ, &indexCirc) && circ[indexCirc] != '\0') {
        //salto gli spazi
        while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') { indexCirc++; }
        //controllo se il valore trovao non e' un accapo oppure fine file e che non si ecceda il limitre di matrice
        while (circ[indexCirc] != '\n' && circ[indexCirc] != '\0' && indexOrder < MAX_MATRIX ) {
            notValid = false;
            while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') { indexCirc++; }
            //se c'e' scritto qualcosa
            if (circ[indexCirc] != '\n' &&circ[indexCirc] != '\0') {
                //per ogni cosa scritta copio nella matrice dei nomi in ordine di ritrovamento
                while (circ[indexCirc] != ' ' && circ[indexCirc] != '\n' && circ[indexCirc] != '\0' && circ[indexCirc] != '\t' && indexName < MAX_NAME_MATRIX) {
                    order[indexOrder][indexName++] = circ[indexCirc++];
                }
                //prossimo nome di circuito
                indexOrder++;
                indexName = 0;
            }
        }
    }
    //se non trovi nulla
    if (indexOrder == 0) {
        return false;
    }

    return true;
}

//questa funzione cerca di ottenere un vettore di matrici dato l'ordine con cui sono
//state dichiarate
Complex*** getMatrix(const int dimention, const char* circ, const int nMatrix , char order[MAX_MATRIX][MAX_NAME_MATRIX]) {
    //inizializzo la matrice di ritorno
    Complex*** orderMatrix = createMatrix3D(nMatrix,dimention,dimention);
    if (!orderMatrix) return NULL;

    int indexOrder = 0, indexCirc = 0;
    //per ogni define trovato fin quando non eccedo il numero di matrici che mi interessano
    while (isPresent("#define", circ, &indexCirc) && indexOrder < nMatrix) {
        //inizializzo il buffer
        int indexBuffer = 0;
        char bufferName[MAX_NAME_MATRIX] = {0};
        //salto gli spazi
        while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') { indexCirc++; }

        //copio il nome
        while ( circ[indexCirc] != ' ' && circ[indexCirc] != '\n' && circ[indexCirc] != '\0' && circ[indexCirc] != '\t' ) {
            bufferName[indexBuffer++] = circ[indexCirc++];
        }

        //controllo se e' quello che mi interessa
        if (strcmp(order[indexOrder], bufferName) == 0) {
            //booleana che mi controlla se i dati sono validi
            bool isValid = true;
            while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') { indexCirc++; }
            //controllo se inizia una marice
            if (circ[indexCirc++] == '[') {
                //inizializzo un buffer per le operazioni
                char bufferOperation[MAX_NAME_MATRIX] = {0};
                int indexOperationBuffer = 0;
                //inizializzo le righe e colonne per inserire i valori nella matrice
                int row = 0, column = 0;

                //fin quando la matrice non si chiude
                while (circ[indexCirc] != ']' && isValid) {
                    //inizializzo per ogni ')' le colonne a 0
                    column = 0;
                    while (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') { indexCirc++; }

                    //se inizia la riga
                    if (circ[indexCirc] == '(') {
                        //fin quando non finisce la riga
                        while (circ[indexCirc] != ')' && isValid) {
                            indexCirc++;
                            //controllo se i caratteri sono legali per l'inserimaneto in matrice
                            if (circ[indexCirc] == '+' || circ[indexCirc] == '-' || circ[indexCirc] == 'i' || circ[indexCirc] == '.' || isdigit(circ[indexCirc])) {
                                bufferOperation[indexOperationBuffer++] = circ[indexCirc];
                            //se hai preso correttamente il dato
                            }else if ((circ[indexCirc] == ',' || circ[indexCirc] == ')') && bufferOperation[0] != '\0'){
                                //controlla se le dimensioni siano rispettate
                                if (column < dimention && row < dimention) {
                                    //printf("%.4f\n%s\n", stringToComplex(bufferOperation).img, bufferOperation);

                                    //inserisci il dato in matrice come numero complesso
                                    orderMatrix[indexOrder][row][column] = stringToComplex(bufferOperation);

                                    //printf("Re: %.4f, img: %.4f \n", orderMatrix[indexOrder][row][column].real, orderMatrix[indexOrder][row][column].img);

                                    //ripulisci il buffer
                                    memset(&bufferOperation, 0, sizeof(bufferOperation));
                                    indexOperationBuffer = 0;
                                }
                                column++;
                                //controlli se e' la fine di una riga
                                if (circ[indexCirc] == ')') {
                                    //controlli che le dimensioni siano rispettate
                                    if (dimention != column) isValid = false;
                                }
                            }else if (circ[indexCirc] == ' ' || circ[indexCirc] == '\t') {
                                //ignori
                            }else {
                                //dato non valido
                                isValid = false;
                                break;
                            }
                        }
                        indexCirc++;

                    }else {
                        //dato non valido
                        isValid = false;
                        break;
                    }
                    //vai alla prossima riga
                    row++;
                }
                //controlli se le dimensioni sono rispettate
                if (dimention != row) isValid = false;
            }

            //se il dato è valido allora puoi andare con la prossima matrice
            if (isValid) {
                indexOrder++;
                //si riinizializza a 0 l'indice di ricerca cosi da poter ricontrollare
                //quello che aveva gia passato
                indexCirc = 0;
            }
        }
    }
    //se non sono state inserite tutte le matrici nella matrice
    if (indexOrder != nMatrix) {
        //libero la memoria
        freeMatrix3D(orderMatrix, nMatrix, dimention);
        return NULL;
    }

    return orderMatrix;
}


//----------------------------------------------------------------------------------------------------------
//----------------------------MATRIX DECLARATIONS & OPERATION FUNCTIONS-------------------------------------
//----------------------------------------------------------------------------------------------------------

//questa funzione si occupa di fare la moltiplicazione tra matrici
Complex** matrixMoltiplication(Complex** complex1, Complex** complex2, const int dim) {
    Complex** ris = createMatrix2D(dim, dim);

    //moltiplico le due matrici secondo le regole dell algebra classica
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

//questa funzione moltiplica una matrice per un vettore
Complex* mulMatrixByVector(Complex** matrix, const Complex* vector, const int dim) {
    Complex* ris = malloc(dim * sizeof(Complex));
    if (!ris) {
        return NULL;
    }

    //moltiplico una matrice per un vettore secondo le regole dell'algebra base
    for (int i = 0; i < dim; i++) {
        Complex sum = {0,0};
        for (int j = 0; j < dim; j++) {
            Complex temp = mul(&matrix[i][j], &vector[j]);
            sum = add(&sum, &temp);
        }
        ris[i] = sum;
    }

    return ris;
}

//questa funzione alloca nell'heap una matrice di Complex 2D
Complex** createMatrix2D(int x, int y) {
    Complex** matrix = malloc(x * sizeof(Complex*));

    if (!matrix) return NULL;
    for (int i = 0; i<x; i++) {
        matrix[i] = malloc(y * sizeof(Complex));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

//questa funzione libera nell'heap una matrice di Complex 2D
void freeMatrix2D(Complex** matrix, int dim) {
    if (!matrix) return;

    for (int i = 0; i < dim; i++) { if (matrix[i]) free(matrix[i]); }
    free(matrix);
}

//questa funzione libera nell'heap una matrice di Complex 3D
void freeMatrix3D(Complex*** m, int x, int y) {
    if (!m) return;
    //libero tutta la matrice
    for (int i = 0; i < x; i++) {
        if (!m[i]) continue;
        for (int j = 0; j < y; j++) {
            free(m[i][j]);
        }
        free(m[i]);
    }
    free(m);
}

//questa funzione alloca nell'heap una matrice di Complex 3D
Complex*** createMatrix3D(int x, int y, int z) {
    //alloco la prima dimensione in memoria
    Complex*** m = malloc(x * sizeof(Complex**));
    if (!m) return NULL;

    for (int i = 0; i < x; i++) {
        //per ogni prima dimensione alloco la seconda dimensione
        m[i] = malloc(y * sizeof(Complex*));
        if (!m[i]) {
            // libero quello allocato fino ad ora
            for (int i2 = 0; i2 < i; i2++) free(m[i2]);
            free(m);
            return NULL;
        }

        for (int j = 0; j < y; j++) {
            //per ogni seconda dimensione alloco la terza dimensione
            m[i][j] = malloc(z * sizeof(Complex));
            if (!m[i][j]) {
                // libero tutto fino ad ora (i e j)
                for (int j2 = 0; j2 < j; j2++) free(m[i][j2]);
                free(m[i]);
                for (int ii = 0; ii < i; ii++) {
                    for (int jj = 0; jj < y; jj++) free(m[ii][jj]);
                    free(m[ii]);
                }
                free(m);
                return NULL;
            }
            //una volta creata la 3 dimensione la inizializzo
            for (int k = 0; k < z; k++) {
                m[i][j][k].real = 0.0;
                m[i][j][k].img = 0.0;
            }
        }
    }

    return m;
}

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

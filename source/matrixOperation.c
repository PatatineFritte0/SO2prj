#include "../header/matrixOperation.h"

#include <stdlib.h>

/* questa funzione prende in input una struttura e tramite quella riesce a moltiplicare
   matrici in sequenza dato un vettore di matrici
   in output da una matrice risultante */
void *mulMatrixThreadFunction(void *args) {
    struct argThreadMatrMoltiplication *arg;
    int indexInit, dim, competence;
    Complex ***circuit;
    Complex **risMul;
    int i, j;

    /* ottengo i paramentri in input */
    arg = (struct argThreadMatrMoltiplication *) args;
    indexInit = arg->indexInit;
    dim = arg->dimMatrix;
    competence = arg->competence;
    circuit = arg->matrix;

    /* creo una matrice identita' */
    risMul = createMatrix2D(dim, dim);
    if (!risMul) return NULL;

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            if (i == j) {
                risMul[i][j].real = 1.0;
                risMul[i][j].img = 0.0;
            } else {
                risMul[i][j].real = 0.0;
                risMul[i][j].img = 0.0;
            }
        }
    }

    /* faccio la moltiplicazione da n+k-1 a n */
    for (i = indexInit + competence - 1; i >= indexInit; i--) {
        Complex **temp = matrixMoltiplication(risMul, circuit[i], dim);
        freeMatrix2D(risMul, dim);
        risMul = temp;
    }

    return (void *)risMul;
}

/* questa funzione si occupa di fare la moltiplicazione tra matrici */
Complex** matrixMoltiplication(Complex** complex1, Complex** complex2, const int dim) {
    int i, j, k;
    Complex **ris;
    ris = createMatrix2D(dim, dim);
    if (!ris) return NULL;

    /* moltiplico le due matrici secondo le regole dell algebra classica */
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            Complex sum;
            sum.real = 0.0;
            sum.img = 0.0;

            for (k = 0; k < dim; k++) {
                Complex temp = mul(&complex1[i][k], &complex2[k][j]);
                sum = add(&sum, &temp);
            }

            ris[i][j] = sum;
        }
    }

    return ris;
}

/* questa funzione moltiplica una matrice per un vettore */
Complex* mulMatrixByVector(Complex** matrix, const Complex* vector, const int dim) {
    int i, j;
    Complex *ris = (Complex*) malloc(dim * sizeof(Complex));
    if (!ris) return NULL;

    /* moltiplico una matrice per un vettore secondo le regole dell'algebra base */
    for (i = 0; i < dim; i++) {
        Complex sum;
        sum.real = 0.0;
        sum.img = 0.0;

        for (j = 0; j < dim; j++) {
            Complex temp = mul(&matrix[i][j], &vector[j]);
            sum = add(&sum, &temp);
        }

        ris[i] = sum;
    }

    return ris;
}

/* questa funzione alloca nell'heap una matrice di Complex 2D */
Complex** createMatrix2D(int x, int y) {
    int i, j;
    Complex **matrix = (Complex**) malloc(x * sizeof(Complex*));
    if (!matrix) return NULL;

    for (i = 0; i < x; i++) {
        matrix[i] = (Complex*) malloc(y * sizeof(Complex));
        if (!matrix[i]) {
            for (j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

/* questa funzione libera nell'heap una matrice di Complex 2D */
void freeMatrix2D(Complex** matrix, int dim) {
    int i;
    if (!matrix) return;

    for (i = 0; i < dim; i++) {
        if (matrix[i]) free(matrix[i]);
    }

    free(matrix);
}

/* questa funzione libera nell'heap una matrice di Complex 3D */
void freeMatrix3D(Complex*** m, int x, int y) {
    int i, j;
    if (!m) return;

    /* libero tutta la matrice */
    for (i = 0; i < x; i++) {
        if (!m[i]) continue;
        for (j = 0; j < y; j++) {
            if (m[i][j]) free(m[i][j]);
        }
        free(m[i]);
    }

    free(m);
}

/* questa funzione alloca nell'heap una matrice di Complex 3D */
Complex*** createMatrix3D(int x, int y, int z) {
    int i, j, k;
    int i2, j2, ii, jj;
    Complex*** m;

    /* alloco la prima dimensione in memoria */
    m = (Complex***) malloc(x * sizeof(Complex**));
    if (!m) return NULL;

    for (i = 0; i < x; i++) {
        /* per ogni prima dimensione alloco la seconda dimensione */
        m[i] = (Complex**) malloc(y * sizeof(Complex*));
        if (!m[i]) {
            /* libero quello allocato fino ad ora */
            for (i2 = 0; i2 < i; i2++) free(m[i2]);
            free(m);
            return NULL;
        }

        for (j = 0; j < y; j++) {
            /* per ogni seconda dimensione alloco la terza dimensione */
            m[i][j] = (Complex*) malloc(z * sizeof(Complex));
            if (!m[i][j]) {
                /* libero tutto fino ad ora (i e j) */
                for (j2 = 0; j2 < j; j2++) free(m[i][j2]);
                free(m[i]);
                for (ii = 0; ii < i; ii++) {
                    for (jj = 0; jj < y; jj++) free(m[ii][jj]);
                    free(m[ii]);
                }
                free(m);
                return NULL;
            }
            /* una volta creata la 3 dimensione la inizializzo */
            for (k = 0; k < z; k++) {
                m[i][j][k].real = 0.0;
                m[i][j][k].img = 0.0;
            }
        }
    }
    return m;
}


/* copia il contenuto da m2 a m1 */
void copyMatrix(Complex** m1, Complex** m2, int row, int col) {
    int i, j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            m1[i][j].real = m2[i][j].real;
            m1[i][j].img = m2[i][j].img;
        }
    }
}


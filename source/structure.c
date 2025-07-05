#include "../header/structure.h"
#include <math.h>

/* qui ci sono tutte le funzioni per le operazioni struct Complex */

/* questa funzione fa la somma di due numeri complessi */
Complex add(const Complex *c1, const Complex *c2) {
    Complex ris = {0, 0};
    ris.real = c1->real + c2->real;
    ris.img = c1->img + c2->img;
    return ris;
}

/* questa funzione fa la moltiplicazione tra 2 numeri complessi */
Complex mul(const Complex *c1, const Complex *c2) {
    Complex ris = {0, 0};
    ris.real = (c1->real * c2->real) - (c1->img * c2->img);
    ris.img = (c1->real * c2->img) + (c1->img * c2->real);
    return ris;
}

/* questa funzione fa il modulo di un numero complesso */
double mod(const Complex c1) {
    return sqrt(pow(c1.img, 2) + pow(c1.real, 2));
}


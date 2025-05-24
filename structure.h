#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <math.h>
//----------------------------------------------------------------------------------------------------------
//--------------------------------------STRUCT DECLARATION--------------------------------------------------
//----------------------------------------------------------------------------------------------------------

typedef struct{
    double real;
    double img;
} Complex;

//----------------------------------------------------------------------------------------------------------
//--------------------------------------OPERATION FUNCTION WITH STRUCT DATA---------------------------------
//----------------------------------------------------------------------------------------------------------

inline Complex add(Complex *c1, Complex *c2) {
    Complex ris = {};

    ris.real = c1->real + c2->real;
    ris.img = c1->img + c2->img;

    return ris;
}

inline Complex mul(Complex *c1, Complex *c2) {
    Complex ris = {};

    ris.real = (c1->real * c1->img) - (c2->img * c2->real);
    ris.img = (c1->real * c2->img) - (c1->img * c2->real);

    return ris;
}

inline double mod(Complex c1) {

    return sqrt(pow(c1.img, 2) + pow(c1.real, 2));
}
#endif //STRUCTURE_H

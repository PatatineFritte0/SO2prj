#ifndef STRUCTURE_H
#define STRUCTURE_H

/*----------------------------------------------------------------------------------------------------------
--------------------------------------STRUCT DECLARATION--------------------------------------------------
----------------------------------------------------------------------------------------------------------*/

typedef struct Complex {
    double real;
    double img;
} Complex;

struct argThreadMatrMoltiplication {
    int dimMatrix;
    int competence;
    int indexInit;
    Complex ***matrix;
};

/*----------------------------------------------------------------------------------------------------------
----------------------------OPERATION PROTOTYPE FUNCTION WITH STRUCT DATA---------------------------------
----------------------------------------------------------------------------------------------------------*/

Complex add(const Complex *c1, const Complex *c2);
Complex mul(const Complex *c1, const Complex *c2);
double mod(Complex c);

#endif /* STRUCTURE_H */


#ifndef STRUCTURE_H
#define STRUCTURE_H
//----------------------------------------------------------------------------------------------------------
//--------------------------------------STRUCT DECLARATION--------------------------------------------------
//----------------------------------------------------------------------------------------------------------

typedef struct{
    double real;
    double img;
} Complex;

struct argThreadMatrMoltiplication {
    int dimMatrix;
    int competence;
    int indexInit;
    Complex*** matrix;
};

//----------------------------------------------------------------------------------------------------------
//----------------------------OPERATION PROTOTYPE FUNCTION WITH STRUCT DATA---------------------------------
//----------------------------------------------------------------------------------------------------------

Complex add(const Complex*, const Complex*);
Complex mul(const Complex*, const Complex*);
double mod(const Complex);

#endif //STRUCTURE_H

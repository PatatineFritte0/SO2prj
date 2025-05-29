#include "../header/structure.h"
#include <math.h>

Complex add(const Complex *c1, const Complex *c2) {
    Complex ris = {0,0};
    ris.real = c1->real + c2->real;
    ris.img = c1->img + c2->img;
    return ris;
}

Complex mul(const Complex *c1, const Complex *c2) {
    Complex ris = {0, 0};
    ris.real = (c1->real * c2->real) - (c1->img * c2->img);
    ris.img  = (c1->real * c2->img) + (c1->img * c2->real);
    return ris;
}

double mod(const Complex c1) {
    return sqrt(c1.img * c1.img + c1.real * c1.real);
}

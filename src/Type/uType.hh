/* 
I'm including a name class to eventually protect this against inserting code into the name field
used throughout the code, and to implement simple bool checking with case-insensitivity. 
*/

#ifndef UTYPE_HH
#define UTYPE_HH

#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include "../Math/vectorMath.hh"

template <int dim>
class UType {
private:
    double u0,u2;
    Lin::Vector<dim> u1;
public:
    UType() {};

    UType(double u0, Lin::Vector<dim> u1, double u2) :
        u0(u0), u1(u1), u2(u2) {}

    double
    getU0() { return u0; }

    Lin::Vector<dim> 
    getU1() { return u1; }

    double
    getU2() { return u2; }
};

#endif
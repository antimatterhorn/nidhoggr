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
    getU0() const { return u0; }

    Lin::Vector<dim> 
    getU1() const { return u1; }

    double
    getU2() const { return u2; }

    UType<dim> 
    operator*(const double other) const {
        return UType<dim>(u0*other,u1*other,u2*other);
    }

    UType<dim> 
    operator+(const UType<dim>& other) const {
        return add(other);
    }

    UType<dim> 
    add(const UType<dim>& other) const {
        UType<dim> result = UType<dim>(u0+other.getU0(),u1+other.getU1(),u2+other.getU2());   
        return result;
    }

    void
    setU0(const double val) { u0 = val; }

    void
    setU1(const Lin::Vector<dim> val) { u1 = val; }

    void
    setU2(const double val) { u2 = val; }
};

#endif
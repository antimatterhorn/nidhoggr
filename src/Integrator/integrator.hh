#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <functional>
#include <vector>
#include <cstddef>
#include "../DataBase/field.hh"

// Define a type alias for the derivative function
using DerivativeFunction = std::function<Field<double>(double, const Field<double>&)>;

class Integrator {
public:
    DerivativeFunction DxDt;
    
    Integrator (const DerivativeFunction& derivativeFunc) : DxDt(derivativeFunc) {}

    ~Integrator() {}

    virtual Field<double> integrate(const Field<double>& initialState, double t, double dt) {
        // fwd euler
        Field<double> nextState = initialState + DxDt(t,initialState)*dt;
        return nextState;
    }
protected:

};

#endif // INTEGRATOR_HH
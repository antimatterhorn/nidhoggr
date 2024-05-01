#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <functional>
#include <vector>
#include <cstddef>
#include "../DataBase/field.hh"
#include "../Math/vectorMath.hh"


template <typename T>
class Integrator {
public:
    using DerivativeFunction = std::function<Field<T>(T, const Field<T>&)>;
    DerivativeFunction DxDt;
    
    Integrator (const DerivativeFunction& derivativeFunc) : DxDt(derivativeFunc) {}

    ~Integrator() {}

    virtual Field<T> 
    integrate(const Field<T>& initialState, double t, double dt) {
        Field<T> nextState = initialState + DxDt(t,initialState)*dt;
        return nextState;
    }
protected:

};

#endif // INTEGRATOR_HH
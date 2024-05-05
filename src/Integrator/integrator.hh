#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <functional>
#include <vector>
#include <cstddef>
#include "../DataBase/field.hh"
#include "../Math/vectorMath.hh"
#include "../State/state.hh"


class Integrator {
public:
    State state;
    
    Integrator (const State& state) : state(state) {}

    ~Integrator() {}

    template <typename T>
    Field<T> 
    integrate(const Field<T>& initialState, 
                const std::function<Field<T>(T, 
                const Field<T>&)>& deriv, 
                double t, double dt) {
        Field<T> nextState = initialState + deriv(t,initialState)*dt;
        return nextState;
    }
protected:

};

#endif // INTEGRATOR_HH
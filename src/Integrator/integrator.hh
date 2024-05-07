#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <functional>
#include <vector>
#include <cstddef>
#include "../DataBase/field.hh"
#include "../Math/vectorMath.hh"
#include "../State/state.hh"
#include "../Physics/physics.hh"

template <int dim>
class Physics; // forward declaration

template <int dim>
class Integrator {
public:
    Integrator (const Physics<dim>* physics) : physics(physics) {}

    ~Integrator() {}

    virtual
    Field<double> 
    integrateScalar(const Field<double>& initialState, 
                const std::function<Field<double>(double, const Field<double>&)>& deriv, 
                double t, double dt) {
        Field<double> nextState = initialState + deriv(t,initialState)*dt;
        return nextState;
    }

    virtual
    Field<Lin::Vector<dim>> 
    integrateVector(const Field<Lin::Vector<dim>>& initialState, 
                const std::function<Field<Lin::Vector<dim>>(Lin::Vector<dim>, const Field<Lin::Vector<dim>>&)>& deriv, 
                double t, double dt) {
        Field<Lin::Vector<dim>> nextState = initialState + deriv(t+dt,initialState)*dt;
        return nextState;
    }
protected:
    const Physics<dim>* physics;
};

#endif 
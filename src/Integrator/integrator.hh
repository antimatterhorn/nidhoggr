#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <functional>
#include <vector>
#include <cstddef>
#include <iostream>
#include "../DataBase/field.hh"
#include "../Math/vectorMath.hh"
#include "../State/state.hh"
#include "../Physics/physics.hh"

template <int dim>
class Physics; // forward declaration

template <int dim>
class Integrator {
protected:
    Physics<dim>* physics;
    double time;
public:
    Integrator (Physics<dim>* physics) : physics(physics) {}

    ~Integrator() {}

    virtual void
    step(double dt) {
        std::cout << "in step";
        for (FieldBase* field : physics->derivFields) {
            std::cout << "in loop";
            if (typeid(*field) == typeid(Field<double>)) {
                Field<double> initialState = this->integrate(initialState,time,dt);
            } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
                Field<Lin::Vector<dim>> initialState = this->integrate(initialState,time,dt);
            }
        }
        time += dt;
    }

    virtual
    Field<double> 
    integrate(const Field<double>& initialState, 
                double t, double dt) {
        //Field<double> nextState = initialState + deriv(t,initialState)*dt;
        Field<double>* interimState = physics->EvaluateDerivatives(initialState,t+dt);
        Field<double> nextState = initialState;
        std::cout << "nextState created";
        if (interimState) {
            Field<double>& field = *interimState;
            nextState = nextState + field*dt;
        }
        return nextState;
    }

    virtual
    Field<Lin::Vector<dim>> 
    integrate(const Field<Lin::Vector<dim>>& initialState, 
                double t, double dt) {
        //Field<Lin::Vector<dim>> nextState = initialState + deriv(t+dt,initialState)*dt;
        Field<Lin::Vector<dim>>* interimState = physics->EvaluateDerivatives(initialState,t+dt);
        Field<Lin::Vector<dim>> nextState = initialState;
        if (interimState) {
            Field<Lin::Vector<dim>>& field = *interimState;
            nextState = nextState + field*dt;
        }
        return nextState;
    }
};

#endif 
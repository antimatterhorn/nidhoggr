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
        std::cout << "in step" << std::endl;
        for (FieldBase* field : physics->derivFields) {
            std::cout << "in loop " << field->getNameString() << " " << field->size() <<  std::endl;
            if (typeid(*field) == typeid(Field<double>)) {
                Field<double>* doubleField = dynamic_cast<Field<double>*>(field);
                if (doubleField)
                    Field<double> initialState = this->integrate(doubleField,time,dt);
            } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
                std::cout << "in vector cast" << std::endl;
                Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
                std::cout << "about to integrate" << std::endl;
                if (vectorField)
                    Field<Lin::Vector<dim>> initialState = this->integrate(vectorField,time,dt);
            }
        }
        time += dt;
    }

    virtual
    Field<double> 
    integrate(const Field<double>* initialState, 
                double t, double dt) {
        //Field<double> nextState = initialState + deriv(t,initialState)*dt;
        Field<double> interimState;
        physics->EvaluateDerivatives(initialState,interimState,t+dt);
        Field<double> nextState;
        //=initialState;
        std::cout << "nextState created";
        nextState = nextState + interimState*dt;
        return nextState;
    }

    virtual
    Field<Lin::Vector<dim>> 
    integrate(const Field<Lin::Vector<dim>>* initialState, 
                double t, double dt) {
        //Field<Lin::Vector<dim>> nextState = initialState + deriv(t+dt,initialState)*dt;
        std::cout << "in integrate" << " " << initialState->getNameString() << " " << initialState->size() <<  std::endl;
        Field<Lin::Vector<dim>> interimState;
        std::cout << "in interim" << " " << std::endl;
        physics->EvaluateDerivatives(initialState,interimState,t+dt);
        
        Field<Lin::Vector<dim>> nextState ;
        //= initialState;
        
        nextState = nextState + interimState*dt;
        return nextState;
    }
};

#endif 
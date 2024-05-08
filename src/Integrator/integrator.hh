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
        for (FieldBase* field : physics->derivFields) {
            if (typeid(*field) == typeid(Field<double>)) {
                Field<double>* doubleField = dynamic_cast<Field<double>*>(field);
                if (doubleField) {
                    Field<double> DxDt = this->Derivative(doubleField,time,dt);
                    doubleField->copyValues(*doubleField + DxDt*dt);
                    // this is forward euler integration
                }
            } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
                Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
                if (vectorField) {
                    Field<Lin::Vector<dim>> DxDt = this->Derivative(vectorField,time,dt);
                    vectorField->copyValues(*vectorField + DxDt*dt);
                }
            }
        }
        time += dt;
    }

    virtual
    Field<double> 
    Derivative(const Field<double>* initialState, 
                double t, double dt) {
        Field<double> deriv = Field<double>("deriv"+initialState->getNameString(),initialState->size());
        physics->EvaluateDerivatives(initialState,deriv,dt);
        return deriv;
    }

    virtual
    Field<Lin::Vector<dim>> 
    Derivative(const Field<Lin::Vector<dim>>* initialState, 
                double t, double dt) {
        Field<Lin::Vector<dim>> deriv = Field<Lin::Vector<dim>>("deriv"+initialState->getNameString(),initialState->size());
        physics->EvaluateDerivatives(initialState,deriv,dt);
        return deriv;
    }
};

#endif 
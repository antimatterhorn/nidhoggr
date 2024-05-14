#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <functional>
#include <vector>
#include <cstddef>
#include <iostream>
#include "../DataBase/field.hh"
#include "../Math/vectorMath.hh"
#include "../State/state.hh"
#include "../Boundaries/boundaries.hh"
#include "../Type/uType.hh"

template <int dim>
class Physics; // forward declaration

template <int dim>
class Integrator {
protected:
    Physics<dim>* physics;
    std::vector<Boundaries<dim>*> boundaries;
    unsigned int cycle;
    double time, dt, dtmin;
public:
    Integrator (Physics<dim>* physics, double dtmin, std::vector<Boundaries<dim>*> boundaries) : 
        physics(physics),
        boundaries(boundaries),
        dt(dtmin),
        dtmin(dtmin){
        cycle = 0;
    }

    ~Integrator() {}

    virtual void
    Step() {
        physics->PreStepInitialize();

        // State* state = physics->getState();
        // State derivs = State(state);
        // NodeList* dvNode = derivs.getNodeList();
        // // at this point i can do copies or whatever based on the integrator
        // physics->EvaluateDerivatives(state,&derivs,dt);

        // for (FieldBase* field : state->fields) {
        //     if (typeid(*field) == typeid(Field<double>)) {
        //         Field<double>* doubleField = dynamic_cast<Field<double>*>(field);
        //         if (doubleField) {
        //             std::string name = doubleField->getNameString();
        //             name = "D" + name;
        //             Field<double>* DxDt = dvNode->getField<double>(name);
        //             doubleField->copyValues(*doubleField + *DxDt*dt);
        //             // this is forward euler integration
        //         }
        //     } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
        //         Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
        //         if (vectorField) {
        //             std::string name = vectorField->getNameString();
        //             name = "D" + name;
        //             Field<Lin::Vector<dim>>* DxDt = dvNode->getField<Lin::Vector<dim>>(name);
        //             vectorField->copyValues(*vectorField + *DxDt*dt);
        //         }
        //     }
        // }

        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries();

        physics->FinalizeStep();

        time += dt;
        cycle+=1;
        
        double newdt = physics->EstimateTimestep();
        dt = std::max(newdt,dtmin);
    }

    // virtual 
    // Field<double> 
    // Derivative(const Field<double>* initialState, 
    //             double t, double dt) {
    //     Field<double> deriv = Field<double>("deriv"+initialState->getNameString(),initialState->size());
    //     physics->EvaluateDerivatives(initialState,deriv,dt);
    //     return deriv;
    // }

    // virtual
    // Field<Lin::Vector<dim>> 
    // Derivative(const Field<Lin::Vector<dim>>* initialState, 
    //             double t, double dt) {
    //     Field<Lin::Vector<dim>> deriv = Field<Lin::Vector<dim>>("deriv"+initialState->getNameString(),initialState->size());
    //     physics->EvaluateDerivatives(initialState,deriv,dt);
    //     return deriv;
    // }

    // virtual
    // Field<UType<dim>> 
    // Derivative(const Field<UType<dim>>* initialState, 
    //             double t, double dt) {
    //     Field<UType<dim>> deriv = Field<UType<dim>>("deriv"+initialState->getNameString(),initialState->size());
    //     physics->EvaluateDerivatives(initialState,deriv,dt);
    //     return deriv;
    // }

    virtual double const Time() { return time;}

    virtual unsigned int Cycle() { return cycle; }

    virtual double const Dt() { return dt; }
};

#endif 
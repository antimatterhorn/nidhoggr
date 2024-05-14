#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <vector>
#include <iostream>
#include "../Math/vectorMath.hh"
#include "../State/state.hh"
#include "../Boundaries/boundaries.hh"

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

        State<dim>* state = physics->getState();
        State<dim> derivatives(state->size());

        // Iterate through the fields in the state
        for (int i = 0; i < state->count(); ++i) {
            FieldBase* field = state->getFieldByIndex(i);
            if (field->hasName()) {
                std::string fieldName = field->getNameString();
                if (dynamic_cast<Field<double>*>(field) != nullptr) {
                    derivatives.template insertField<double>(fieldName);
                } 
                else if (dynamic_cast<Field<Lin::Vector<dim>>*>(field) != nullptr) {
                    derivatives.template insertField<Lin::Vector<dim>>(fieldName);
                }
            }
        }

        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries();

        physics->FinalizeStep();

        time += dt;
        cycle+=1;
        
        double newdt = physics->EstimateTimestep();
        dt = std::max(newdt,dtmin);
    }

    virtual double const Time() { return time;}

    virtual unsigned int Cycle() { return cycle; }

    virtual double const Dt() { return dt; }
};

#endif 
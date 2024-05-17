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

        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries(); 

        State<dim>* state = physics->getState();
        State<dim> derivatives(state->size());

        derivatives.ghost(state);

        physics->EvaluateDerivatives(state,derivatives,time,0);
        // forward euler takes the derivative NOW and multiplies by dt
        derivatives*=dt;

        State<dim> newState(state->size());
        newState.clone(state);
        newState+=derivatives;

        physics->FinalizeStep(&newState);

        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries(); 

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
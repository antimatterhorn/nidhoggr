#ifndef INTEGRATOR_CC
#define INTEGRATOR_CC

#include "integrator.hh"

template <int dim>
Integrator<dim>::Integrator(std::vector<Physics<dim>*> packages, double dtmin)
    : packages(packages), dt(dtmin), dtmin(dtmin), cycle(0), time(0.0) {}

template <int dim>
Integrator<dim>::~Integrator() {}

template <int dim>
void Integrator<dim>::Step() {  
    if (cycle == 0) {
        for (Physics<dim>* physics : packages)
            physics->ZeroTimeInitialize();
    }

    time += dt;
    cycle += 1;

    for (Physics<dim>* physics : packages)
    {
        physics->PreStepInitialize();
        physics->ApplyBoundaries();

        State<dim>* state = physics->getState();
        State<dim> derivatives(state->size());
        derivatives.ghost(state);

        physics->EvaluateDerivatives(state, derivatives, time, 0);

        derivatives *= dt;
        State<dim> newState(state->size());
        newState.ghost(state);
        newState += *state;
        newState += derivatives;

        physics->ApplyBoundaries();
        physics->FinalizeStep(&newState);
    }

    double olddt = 1e30;
    for (Physics<dim>* physics : packages) {
        double newdt = physics->EstimateTimestep();
        newdt = std::min(newdt,olddt);
        dt = std::max(newdt, dtmin);
        olddt = newdt;
    }
}

template <int dim>
double const Integrator<dim>::Time() { return time; }

template <int dim>
unsigned int Integrator<dim>::Cycle() { return cycle; }

template <int dim>
double const Integrator<dim>::Dt() { return dt; }

#endif // INTEGRATOR_CC

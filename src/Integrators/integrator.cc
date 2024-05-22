#ifndef INTEGRATOR_CC
#define INTEGRATOR_CC

#include "integrator.hh"

template <int dim>
Integrator<dim>::Integrator(Physics<dim>* physics, double dtmin, std::vector<Boundaries<dim>*> boundaries)
    : physics(physics), boundaries(boundaries), dt(dtmin), dtmin(dtmin), cycle(0), time(0.0) {}

template <int dim>
Integrator<dim>::~Integrator() {}

template <int dim>
void Integrator<dim>::Step() {
    if (cycle == 0) {
        physics->ZeroTimeInitialize();
    }

    time += dt;
    cycle += 1;

    physics->PreStepInitialize();

    if (!boundaries.empty()) {
        for (Boundaries<dim>* bounds : boundaries) {
            bounds->ApplyBoundaries();
        }
    }

    State<dim>* state = physics->getState();
    State<dim> derivatives(state->size());

    derivatives.ghost(state);
    physics->EvaluateDerivatives(state, derivatives, time, 0);
    derivatives *= dt;

    State<dim> newState(state->size());
    newState.ghost(state);
    newState += *state;
    newState += derivatives;

    physics->FinalizeStep(&newState);

    if (!boundaries.empty()) {
        for (Boundaries<dim>* bounds : boundaries) {
            bounds->ApplyBoundaries();
        }
    }

    double newdt = physics->EstimateTimestep();
    dt = std::max(newdt, dtmin);
}

template <int dim>
double const Integrator<dim>::Time() { return time; }

template <int dim>
unsigned int Integrator<dim>::Cycle() { return cycle; }

template <int dim>
double const Integrator<dim>::Dt() { return dt; }

#endif // INTEGRATOR_CC

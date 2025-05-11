#pragma once

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

    VoteDt();
}

template <int dim>
void Integrator<dim>::VoteDt() {
        double smallestDt = 1e30;
        for (Physics<dim>* physics : packages) {
            double newdt = physics->EstimateTimestep();
            if (newdt < smallestDt) {
                smallestDt = newdt;
                //std::cout << physics->name() << " requested timestep of " << newdt << "\n";
            }
        }
        if (smallestDt < dt)
            dt *= 0.5;
        else
            dt *= 1.2;
        this->dt = std::max(dt, this->dtmin);
}

template <int dim>
double const Integrator<dim>::Time() { return time; }

template <int dim>
unsigned int Integrator<dim>::Cycle() { return cycle; }

template <int dim>
double const Integrator<dim>::Dt() { return dt; }

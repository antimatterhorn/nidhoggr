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
    Integrator(Physics<dim>* physics, double dtmin, std::vector<Boundaries<dim>*> boundaries);
    ~Integrator();

    virtual void Step();
    virtual double const Time();
    virtual unsigned int Cycle();
    virtual double const Dt();
};

#include "integrator.cc"

#endif // INTEGRATOR_HH

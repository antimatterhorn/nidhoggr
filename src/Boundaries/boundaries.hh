#ifndef BOUNDARIES_HH
#define BOUNDARIES_HH

#include <vector>
#include "../Physics/physics.hh"

template <int dim>
class Physics;

// Base class for Grid Boundaries
template <int dim>
class Boundaries {
protected:
    Physics<dim>* physics;
public:
    Boundaries(Physics<dim>* physics) : 
        physics(physics) {

    }
    
    virtual ~Boundaries() {}

    virtual void
    ApplyBoundaries() {}

};

#endif // BOUNDARIES_HH
#ifndef COLLIDER_HH
#define COLLIDER_HH

#include <vector>
#include "../Physics/physics.hh"
#include "boundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class Collider : public Boundaries<dim> {
protected:

public:
    Collider(Physics<dim>* physics) : 
        Boundaries<dim>(physics){}
    
    virtual ~Collider() {}

};

#endif
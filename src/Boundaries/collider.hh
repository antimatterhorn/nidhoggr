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
    Collider(){}
    
    virtual ~Collider() {}

    virtual bool Inside(Lin::Vector<dim>& otherPosition, double otherRadius) { return false; }

};

#endif
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

public:
    Boundaries() {}
    
    virtual ~Boundaries() {
        std::cout << "Destroying Boundaries" << std::endl;
    }

    virtual void
    ApplyBoundaries(State<dim>& state, NodeList* nodeList) {}

};

#endif // BOUNDARIES_HH